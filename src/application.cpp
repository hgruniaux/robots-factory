#include "application.hpp"
#include "robot/robot.hpp"
#include "robot/robot_inspector_ui.hpp"
#include "simulation/simulation_view.hpp"

#include "robot/robot_parser.hpp"
#include "robot/robot_saver.hpp"

#include "shared_library.hpp"

#include <fstream>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <spdlog/spdlog.h>
#include <tinyfiledialogs.h>
#include <nlohmann/json.hpp>

Application::Application() = default;

Application::~Application() {
    unload_robot_ai();
}

bool Application::start() {
    m_robot_inspector_ui = std::make_unique<RobotInspectorUI>();
    m_simulation_view = std::make_unique<SimulationView>();

    load_settings();
    return true;
}

void Application::stop() {
    save_settings();
}

void Application::update() {
    if (ImGui::BeginMainMenuBar()) {
        show_main_menu_bar();
        ImGui::EndMainMenuBar();
    }

    show_new_robot_modal();

    m_robot_inspector_ui->show(m_show_robot_inspector, m_show_robot_part_inspector, m_show_robot_preview);

    if (m_show_robot_ai_inspector)
        show_robot_ai_inspector();

    m_simulation_view->show(m_show_simulation_view);
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

        ImGui::Separator();

        if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save robot", nullptr, nullptr, has_robot))
            save_robot();

        if (ImGui::MenuItem(ICON_FA_FLOPPY_DISK " Save robot as...", nullptr, nullptr, has_robot))
            save_robot_as();

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Default windows")) {
            m_show_robot_inspector = true;
            m_show_robot_part_inspector = true;
            m_show_robot_preview = true;
            m_show_robot_ai_inspector = true;
            m_show_simulation_view = true;
        }

        if (ImGui::MenuItem("Simulation layout")) {
            m_show_robot_inspector = false;
            m_show_robot_part_inspector = false;
            m_show_robot_preview = false;
            m_show_robot_ai_inspector = true;
            m_show_simulation_view = true;
        }

        if (ImGui::MenuItem("Robot layout")) {
            m_show_robot_inspector = true;
            m_show_robot_part_inspector = true;
            m_show_robot_preview = true;
            m_show_robot_ai_inspector = false;
            m_show_simulation_view = false;
        }

        ImGui::Separator();

        ImGui::MenuItem(ICON_FA_ROBOT " Robot inspector", nullptr, &m_show_robot_inspector);
        ImGui::MenuItem(ICON_FA_PUZZLE_PIECE " Robot part inspector", nullptr, &m_show_robot_part_inspector);
        ImGui::MenuItem(ICON_FA_ROBOT " Robot preview", nullptr, &m_show_robot_preview);
        ImGui::MenuItem(ICON_FA_BRAIN " Robot AI inspector", nullptr, &m_show_robot_ai_inspector);
        ImGui::MenuItem(ICON_FA_FLASK " Simulation view", nullptr, &m_show_simulation_view);

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

            ImGui::Separator();

            if (ImGui::MenuItem(ICON_FA_ROTATE_RIGHT " Restart"))
                m_simulation_view->start();
            if (ImGui::MenuItem(ICON_FA_STOP " Stop"))
                m_simulation_view->stop();

            ImGui::Separator();

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
        static std::string name = "My robot";
        static std::string path = "my_robot.yml";
        static bool is_default_path = true;

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Name:");
        ImGui::SameLine();
        if (ImGui::InputText("##name", &name) && is_default_path) {
            path = name;
            for (char &c : path) {
                c = (char)std::tolower(c);
                if (c == ' ')
                    c = '_';
            }

            path = path + ".json";
        }

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Path:");
        ImGui::SameLine();
        if (ImGui::InputText("##path", &path))
            is_default_path = false;

        ImGui::SameLine();
        if (ImGui::Button("...")) {
            const char *filter_patterns[] = {"*.json", "*.yml", "*.yaml"};
            const char *robot_path = tinyfd_saveFileDialog("Choose robot save path", path.c_str(), std::size(filter_patterns), filter_patterns, nullptr);
            if (robot_path != nullptr)
                path = robot_path;
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
    if (ImGui::Begin(ICON_FA_BRAIN " Robot AI", &m_show_robot_ai_inspector)) {
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
    const char *filter_patterns[] = {"*.dll", "*.so", "*.dylib"};
    const char *robot_ai_path = tinyfd_openFileDialog("Load robot AI", "", std::size(filter_patterns), filter_patterns, nullptr, false);
    if (robot_ai_path == nullptr)
        return;

    load_robot_ai(robot_ai_path);
}

void Application::load_robot_ai(const std::string &path) {
    SPDLOG_TRACE("Loading robot AI from '{}'", path);
    // Load shared library
    load_native_robot_ai(path);
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

static constexpr const char *SETTINGS_FILENAME = "settings.json";

void Application::load_settings() {
    nlohmann::json settings;
    std::ifstream stream(SETTINGS_FILENAME);
    if (stream.is_open()) {
        stream >> settings;
    } else {
        return;
    }

    if (settings.contains("Layout")) {
        m_show_robot_inspector = settings["Layout"].value("show_robot_inspector", true);
        m_show_robot_part_inspector = settings["Layout"].value("show_robot_part_inspector", true);
        m_show_robot_preview = settings["Layout"].value("show_robot_preview", true);
        m_show_robot_ai_inspector = settings["Layout"].value("show_robot_ai_inspector", true);
        m_show_simulation_view = settings["Layout"].value("show_simulation_view", true);
    }

    if (settings.contains("LastLoaded")) {
        if (settings["LastLoaded"].contains("robot")) {
            std::string robot_path = settings["LastLoaded"]["robot"];
            if (!robot_path.empty())
                load_robot(robot_path);
        }

        if (settings["LastLoaded"].contains("robot_ai")) {
            std::string robot_ai_path = settings["LastLoaded"]["robot_ai"];
            if (!robot_ai_path.empty())
                load_robot_ai(robot_ai_path);
        }
    }
}

void Application::save_settings() {
    nlohmann::json settings;
    settings["Layout"]["show_robot_inspector"] = m_show_robot_inspector;
    settings["Layout"]["show_robot_part_inspector"] = m_show_robot_part_inspector;
    settings["Layout"]["show_robot_preview"] = m_show_robot_preview;
    settings["Layout"]["show_robot_ai_inspector"] = m_show_robot_ai_inspector;
    settings["Layout"]["show_simulation_view"] = m_show_simulation_view;

    if (m_robot != nullptr && !m_robot->get_source_file().empty())
        settings["LastLoaded"]["robot"] = m_robot->get_source_file();
    if (!m_robot_ai_path.empty())
        settings["LastLoaded"]["robot_ai"] = m_robot_ai_path;

    std::ofstream stream(SETTINGS_FILENAME);
    stream << settings.dump(4);
}
