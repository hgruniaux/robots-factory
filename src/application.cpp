#include "application.hpp"
#include "robot/robot.hpp"
#include "robot/robot_inspector_ui.hpp"
#include "simulation/simulation_view.hpp"

#include "robot/lua_robot_ai.hpp"

#include "robot/robot_parser.hpp"
#include "robot/robot_saver.hpp"

#include "shared_library.hpp"

#include <imgui.h>
#include <spdlog/spdlog.h>
#include <tinyfiledialogs.h>

Application::Application() = default;

Application::~Application() {
    unload_robot_ai();
}

bool Application::start() {
    m_robot_inspector_ui = std::make_unique<RobotInspectorUI>();
    m_simulation_view = std::make_unique<SimulationView>();

    load_robot("robots/spider/robot.yml");
    load_robot_ai("robots/spider/robot.lua");
    return true;
}

void Application::update() {
    if (ImGui::BeginMainMenuBar()) {
        show_main_menu_bar();
        ImGui::EndMainMenuBar();
    }

    show_new_robot_modal();

    m_robot_inspector_ui->show();
    show_robot_ai_inspector();
    m_simulation_view->show();
}

void Application::show_main_menu_bar() {
    bool has_robot = m_robot != nullptr;

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New robot"))
            new_robot();

        if (ImGui::MenuItem("Load robot"))
            load_robot();

        if (ImGui::MenuItem("Load robot AI"))
            load_robot_ai();

        if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save robot", nullptr, nullptr, has_robot))
            save_robot();

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Simulation", has_robot)) {
        if (m_simulation_view->is_started()) {
            if (m_simulation_view->is_paused()) {
                if (ImGui::MenuItem(ICON_FA_PLAY " Resume"))
                    m_simulation_view->resume();
            } else {
                if (ImGui::MenuItem(ICON_FA_PAUSE " Pause"))
                    m_simulation_view->pause();
            }

            if (ImGui::MenuItem(ICON_FA_ROTATE_RIGHT " Restart"))
                m_simulation_view->start();
            if (ImGui::MenuItem(ICON_FA_STOP " Stop"))
                m_simulation_view->stop();

            if (ImGui::MenuItem(ICON_FA_FORWARD_STEP " Step forward"))
                m_simulation_view->step_forward();
            if (ImGui::MenuItem(ICON_FA_FORWARD_FAST " Step forward fast"))
                m_simulation_view->step_forward_fast();
        } else {
            if (ImGui::MenuItem(ICON_FA_PLAY " Start"))
                m_simulation_view->start();
        }

        ImGui::EndMenu();
    }
}

void Application::show_new_robot_modal() {
    if (m_open_new_robot_modal) {
        ImGui::OpenPopup("New robot");
        m_open_new_robot_modal = false;
    }

    if (ImGui::BeginPopupModal("New robot")) {
        static char name[256] = "New robot";
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::InputText("##name", name, std::size(name));

        static char path[256] = "robot.yml";
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Path:");
        ImGui::SameLine();
        ImGui::InputText("##path", path, std::size(path));
        ImGui::SameLine();
        if (ImGui::Button("...")) {
            const char *filter_patterns[] = {"*.json", "*.yml", "*.yaml"};
            const char *robot_path = tinyfd_saveFileDialog("Save robot", path, std::size(filter_patterns), filter_patterns, nullptr);
            if (robot_path != nullptr)
                std::strncpy(path, robot_path, std::size(path));
        }

        if (ImGui::Button("Create")) {
            SPDLOG_INFO("Creating new robot '{}'", name);
            m_robot = std::make_shared<Robot>();
            m_robot->set_name(name);
            m_robot->set_source_file(path);
            m_robot_inspector_ui->set_robot(m_robot);
            m_simulation_view->set_robot(m_robot);

            // Unload the current robot AI
            m_robot_ai = nullptr;
            m_simulation_view->set_robot_ai(nullptr);

            m_simulation_view->pause();
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void Application::show_robot_ai_inspector() {
    if (ImGui::Begin(ICON_FA_BRAIN " Robot AI")) {
        if (m_robot_ai == nullptr) {
            ImGui::Text("No robot AI loaded");
        } else {
            ImGui::Text("Robot AI loaded");

            if (m_robot_ai_path.empty())
                ImGui::Text("Path: <unknown>");
            else
                ImGui::Text("Path: %s", m_robot_ai_path.c_str());

            if (ImGui::Button("Reload"))
                reload_robot_ai();
            ImGui::SameLine();
            if (ImGui::Button("Unload"))
                unload_robot_ai();
        }
    }

    ImGui::End();
}

void Application::new_robot() {
    m_open_new_robot_modal = true;
}

void Application::load_robot() {
    const char *filter_patterns[] = {"*.json", "*.yml", "*.yaml"};
    const char *robot_path = tinyfd_openFileDialog("Load robot", "", std::size(filter_patterns), filter_patterns, nullptr, false);
    if (robot_path == nullptr)
        return;

    load_robot(robot_path);
}

void Application::load_robot(const std::string &path) {
    auto robot = parse_robot(path);
    if (robot == nullptr) {
        SPDLOG_ERROR("Failed to load robot from '{}'", path);
        tinyfd_messageBox("Error", "Failed to load robot", "ok", "error", 1);
        return;
    }

    m_robot = robot;
    m_robot_inspector_ui->set_robot(m_robot);
    m_simulation_view->set_robot(m_robot);

    // Unload the current robot AI
    unload_robot_ai();

    // Pause the simulation
    m_simulation_view->pause();

    SPDLOG_INFO("Robot loaded successfully");
}

void Application::save_robot() {
    if (m_robot == nullptr)
        return;

    if (m_robot->get_source_file().empty())
        save_robot_as();
    else
        save_robot(m_robot->get_source_file());
}

void Application::save_robot(const std::string &path) {
    ::save_robot(m_robot, path);
    SPDLOG_INFO("Robot saved successfully");
}

void Application::save_robot_as() {
    if (m_robot == nullptr)
        return;

    const char *robot_path = tinyfd_saveFileDialog("Save robot as", "", 0, nullptr, nullptr);
    if (robot_path == nullptr)
        return;

    save_robot(robot_path);
}

void Application::load_robot_ai() {
    const char *filter_patterns[] = {"*.dll", "*.so", "*.dylib", "*.lua"};
    const char *robot_ai_path = tinyfd_openFileDialog("Load robot AI", "", std::size(filter_patterns), filter_patterns, nullptr, false);
    if (robot_ai_path == nullptr)
        return;

    load_robot_ai(robot_ai_path);
}

void Application::load_robot_ai(const std::string &path) {
    SPDLOG_TRACE("Loading robot AI from '{}'", path);

    if (path.ends_with(".lua")) {
        // Load Lua script
        load_lua_robot_ai(path);
    } else {
        // Load shared library
        load_native_robot_ai(path);
    }
}

void Application::load_lua_robot_ai(const std::string &path) {
    auto robot_ai = std::make_shared<LuaRobotAI>();
    if (!robot_ai->load_from_file(path)) {
        SPDLOG_ERROR("Failed to load Lua robot AI from '{}'", path);
        tinyfd_messageBox("Error", "Failed to load Lua robot AI", "ok", "error", 1);
        return;
    }

    m_robot_ai = robot_ai;
    m_simulation_view->set_robot_ai(m_robot_ai);
    m_robot_ai_lib.reset();// free the shared library as not needed anymore
    m_robot_ai_path = path;

    // Pause the simulation
    m_simulation_view->pause();

    SPDLOG_INFO("Lua robot AI loaded successfully");
}

using CreateRobotAIFunction = RobotAI *(*) ();

void Application::load_native_robot_ai(const std::string &path) {
    auto library = std::make_unique<SharedLibrary>(path);
    if (!library->load()) {
        SPDLOG_ERROR("Failed to load shared library '{}'", path);
        tinyfd_messageBox("Error", "Failed to load native robot AI", "ok", "error", 1);
        return;
    }

    auto create_robot_ai = library->get<CreateRobotAIFunction>("create_robot_ai");
    if (create_robot_ai == nullptr) {
        SPDLOG_ERROR("Failed to load symbol 'create_robot_ai' from native robot AI");
        tinyfd_messageBox("Error", "Failed to load native robot AI", "ok", "error", 1);
        return;
    }

    auto *robot_ai = create_robot_ai();
    if (robot_ai == nullptr) {
        SPDLOG_ERROR("Failed to create robot AI from native robot AI");
        tinyfd_messageBox("Error", "Failed to create robot AI from native robot AI", "ok", "error", 1);
        return;
    }

    m_robot_ai = std::shared_ptr<RobotAI>(robot_ai);
    m_simulation_view->set_robot_ai(m_robot_ai);
    m_robot_ai_lib = std::move(library);
    m_robot_ai_path = path;

    // Pause the simulation
    m_simulation_view->pause();

    SPDLOG_INFO("Native robot AI loaded successfully");
}

void Application::reload_robot_ai() {
    if (m_robot_ai_path.empty())
        return;

    unload_robot_ai();
    load_robot_ai(m_robot_ai_path);
}

void Application::unload_robot_ai() {
    m_simulation_view->set_robot_ai(nullptr);
    m_robot_ai = nullptr;
    m_robot_ai_lib.reset();
}
