#include "simulation_view.hpp"

#include "IconsFontAwesome6.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <tinyfiledialogs.h>

void SimulationView::set_robot(const std::shared_ptr<Robot> &robot) {
    m_robot = robot;
    restart_if_needed();
}

void SimulationView::set_robot_ai(const std::shared_ptr<RobotAI> &robot_ai) {
    m_robot_ai = robot_ai;
    restart_if_needed();
}

void SimulationView::show(bool& should_show) {
    if (!should_show)
        return;

    handle_shortcuts();

    // Simulate the simulation by one step
    if (m_simulation != nullptr && !m_simulation_paused) {
        // If in real time, only step
        if (m_real_time) {
            m_time_since_last_step += ImGui::GetIO().DeltaTime * m_time_dilatation;
            if (m_time_since_last_step >= m_simulation->get_time_step()) {
                while (m_time_since_last_step >= m_simulation->get_time_step()) {
                    handle_robot_input(m_simulation->get_time_step());
                    m_simulation->step();
                    m_time_since_last_step -= m_simulation->get_time_step();
                }

                m_time_since_last_step = 0.0f;
            }
        } else {
            m_simulation->step();
        }
    }

    if (m_follow_robot)
        center_robot();

    show_robot_info();

    ImGui::PushID(this);
    if (ImGui::Begin(ICON_FA_FLASK " Simulation", &should_show)) {
        if (m_robot != nullptr) {
            show_toolbar();
            show_world();
        } else {
            ImGui::Text("No robot selected...");
        }
    }
    ImGui::End();
    ImGui::PopID();
}

void SimulationView::start() {
    m_simulation = std::make_unique<Simulation>(m_robot, m_robot_ai, m_world_description_path);
    m_time_since_last_step = 0.0f;
}

void SimulationView::stop() {
    m_simulation.reset();
    m_simulation_paused = false;
    m_time_since_last_step = 0.0f;
}

void SimulationView::pause() {
    if (m_simulation == nullptr)
        return;

    m_simulation_paused = true;
    m_time_since_last_step = 0.0f;
}

void SimulationView::resume() {
    if (m_simulation == nullptr)
        return;

    m_simulation_paused = false;
    m_time_since_last_step = 0.0f;
}

void SimulationView::step_forward() {
    if (m_simulation == nullptr)
        return;

    m_simulation->step();
    m_time_since_last_step = 0.0f;
}

void SimulationView::step_forward_fast(float time_in_s) {
    if (m_simulation == nullptr)
        return;

    const float elapsed_time = m_simulation->get_elapsed_time();
    while (m_simulation->get_elapsed_time() - elapsed_time < time_in_s)
        m_simulation->step();
    m_time_since_last_step = 0.0f;
}

void SimulationView::show_toolbar() {
    if (m_simulation == nullptr) {
        if (ImGui::Button(ICON_FA_PLAY))
            start();
        ImGui::SetItemTooltip("Start the simulation");

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_FORWARD_STEP)) {
            start();
            pause();
        }
        ImGui::SetItemTooltip("Start the simulation and pause it immediately");

        ImGui::SameLine();
        ImGui::InputTextWithHint("##world_description", "Path to world description", &m_world_description_path);

        ImGui::SameLine();
        if (ImGui::Button("...")) {
            const char *filter_patterns[] = {"*.json", "*.yml", "*.yaml"};
            const char *world_path = tinyfd_openFileDialog("Open world description", m_world_description_path.c_str(), std::size(filter_patterns), filter_patterns, nullptr, 0);
            if (world_path != nullptr)
                m_world_description_path = world_path;
        }
    } else {
        if (is_paused()) {
            if (ImGui::Button(ICON_FA_PLAY))
                resume();
            ImGui::SetItemTooltip("Resume the simulation");
        } else {
            if (ImGui::Button(ICON_FA_PAUSE))
                pause();
            ImGui::SetItemTooltip("Pause the simulation");
        }

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_ROTATE_RIGHT))
            start();
        ImGui::SetItemTooltip("Restart the simulation");

        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_STOP))
            stop();
        ImGui::SetItemTooltip("Stop the simulation");

        if (m_simulation_paused) {
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_FORWARD_STEP))
                step_forward();
            ImGui::SetItemTooltip("Step forward one step");

            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_FORWARD_FAST))
                step_forward_fast();
            ImGui::SetItemTooltip("Step forward one second");
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::CalcTextSize(ICON_FA_CLOCK " xxxxxxx").x);
        ImGui::DragFloat(ICON_FA_CLOCK, &m_time_dilatation, 0.001f, 0.001f, 2.0f);
        ImGui::SetItemTooltip("Time dilatation factor");

        // Show the elapsed time
        if (m_simulation != nullptr) {
            // Align the text to the right
            float available_width = ImGui::GetContentRegionAvail().x;
            float text_width = ImGui::CalcTextSize("Elapsed time: 00000.00 s").x;
            ImGui::SameLine(available_width - text_width);
            ImGui::Text("Elapsed time: %.2f s", m_simulation->get_elapsed_time());
        }
    }
}

class ExplosionQueryCallback : public b2QueryCallback {
public:
    b2Vec2 blast_center;
    float blast_power;

    bool ReportFixture(b2Fixture *fixture) override {
        b2Body *body = fixture->GetBody();
        if (body->GetType() != b2_dynamicBody)
            return true;

        b2Vec2 body_center = body->GetWorldCenter();
        b2Vec2 blast_dir = body_center - blast_center;
        float distance = blast_dir.Normalize();
        if (distance == 0)
            return true;

        float inv_distance = 1 / distance;
        float impulse_mag = blast_power * inv_distance * inv_distance;
        body->ApplyLinearImpulse(impulse_mag * blast_dir, body_center, true);

        return true;
    }
};// class ExplosionQueryCallback

void SimulationView::show_world() {
    if (m_simulation == nullptr)
        return;

    m_scene_view.begin();
    m_simulation->draw(m_renderer);
    if (m_robot_ai != nullptr)
        m_robot_ai->debug_draw(m_renderer);
    m_scene_view.end();

    glm::vec2 mouse_world_position;
    if (m_scene_view.is_right_clicked(mouse_world_position) && m_simulation != nullptr && !m_simulation_paused) {
        auto *world = m_simulation->get_world();
        ExplosionQueryCallback callback;
        callback.blast_center = {mouse_world_position.x, mouse_world_position.y};
        callback.blast_power = 0.01f;
        const float explosion_radius = 10.0f;

        b2AABB region = {b2Vec2(mouse_world_position.x - explosion_radius, mouse_world_position.y - explosion_radius),
                         b2Vec2(mouse_world_position.x + explosion_radius, mouse_world_position.y + explosion_radius)};
        world->QueryAABB(&callback, region);
    }
}

void SimulationView::show_robot_info() {

    if (ImGui::Begin("Simulation robot info")) {
        if (m_simulation == nullptr) {
            ImGui::Text("No simulation running...");
            ImGui::End();
            return;
        }

        if (ImGui::CollapsingHeader(ICON_FA_CIRCLE_INFO " Robot AI info", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (m_robot_ai != nullptr) {
                m_robot_ai->show_state();
            } else {
                ImGui::Text("No robot AI attached...");
            }
        }

        if (ImGui::CollapsingHeader(ICON_FA_GAMEPAD " Robot user input", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (m_robot_ai != nullptr) {
                auto &user_input = m_robot_ai->get_user_input();

                auto draw_virtual_joystick = [](float x, float y) {
                    const float virtual_joystick_size = ImGui::GetFontSize() * 2.5f;
                    const auto virtual_joystick_center = ImVec2(ImGui::GetCursorScreenPos().x + virtual_joystick_size / 2.f, ImGui::GetCursorScreenPos().y + virtual_joystick_size / 2.f);
                    ImGui::GetWindowDrawList()->AddCircle(virtual_joystick_center, virtual_joystick_size / 2.f, IM_COL32(255, 255, 255, 255), 30, 2.0f);
                    ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(virtual_joystick_center.x + x * virtual_joystick_size / 2.f,
                                                                       virtual_joystick_center.y + -y * virtual_joystick_size / 2.f),
                                                                virtual_joystick_size / 15.f, IM_COL32(255, 255, 255, 255));
                    ImGui::Dummy(ImVec2(virtual_joystick_size, virtual_joystick_size));
                };

                ImGui::Text("Main axis: (x: %.2f, y: %.2f)", user_input.main_x, user_input.main_y);
                ImGui::Text("Secondary axis: (x: %.2f, y: %.2f)", user_input.secondary_x, user_input.secondary_y);
                draw_virtual_joystick(user_input.main_x, user_input.main_y);
                ImGui::SameLine();
                ImGui::Spacing();
                ImGui::SameLine();
                draw_virtual_joystick(user_input.secondary_x, user_input.secondary_y);
                ImGui::Text("Action A: %s", user_input.action_a ? "true" : "false");
                ImGui::Text("Action B: %s", user_input.action_b ? "true" : "false");
                ImGui::Text("Action X: %s", user_input.action_x ? "true" : "false");
            } else {
                ImGui::Text("No robot AI attached...");
            }
        }
    }
    ImGui::End();
}

void SimulationView::restart_if_needed() {
    if (m_simulation == nullptr)
        return;

    start();
}

void SimulationView::handle_shortcuts() {
    // Pause/resume the simulation
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Space))) {
        if (m_simulation == nullptr) {
            start();
        } else if (m_simulation_paused) {
            resume();
        } else {
            pause();
        }
    }

    // Forward time
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F))) {
        if (ImGui::GetIO().KeyShift)
            step_forward_fast();
        else
            step_forward();
    }
}

#include <GLFW/glfw3.h>
#include <fmt/format.h>
//#include <glfw/glfw3.h>

void SimulationView::handle_robot_input(float dt) {
    int gamepad_jid = GLFW_JOYSTICK_1;

    // Possible user inputs for the user.
    float main_x = 0.0f;
    float main_y = 0.0f;
    float secondary_x = 0.0f;
    float secondary_y = 0.0f;
    float third_x = 0.0f;
    float third_y = 0.0f;
    bool action_a = false;
    bool action_b = false;
    bool action_y = false;
    bool action_x = false;
    bool action_lb = false;
    bool action_rb = false;

    // Handle gamepad input
    GLFWgamepadstate gamepad_state;
    if (glfwGetGamepadState(gamepad_jid, &gamepad_state)) {
        action_a = gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS;
        action_b = gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS;
        action_x = gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS;
        action_y = gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS;
        action_lb = gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS;
        action_rb = gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS;

        float left_axis_x = gamepad_state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
        float left_axis_y = -gamepad_state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
        float right_axis_x = gamepad_state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
        float right_axis_y = -gamepad_state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

        float dpad_axis_x = 0.0f, dpad_axis_y = 0.0f;
        if (gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS)
            dpad_axis_x -= 1.0f;
        if (gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS)
            dpad_axis_x += 1.0f;
        if (gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS)
            dpad_axis_y += 1.0f;
        if (gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS)
            dpad_axis_y -= 1.0f;

        // Around 0, the axis is not perfectly centered, so we consider it as 0.
        const float axis_dead_zone = 0.1f;
        if (std::abs(left_axis_x) <= axis_dead_zone)
            left_axis_x = 0.0f;
        if (std::abs(left_axis_y) <= axis_dead_zone)
            left_axis_y = 0.0f;
        if (std::abs(right_axis_x) <= axis_dead_zone)
            right_axis_x = 0.0f;
        if (std::abs(right_axis_y) <= axis_dead_zone)
            right_axis_y = 0.0f;

        // Register robot inputs
        main_x = right_axis_x;
        main_y = right_axis_y;
        secondary_x = left_axis_x;
        secondary_y = left_axis_y;
        third_x = dpad_axis_x;
        third_y = dpad_axis_y;
    }

    // Handle keyboard inputs
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_W)))
        main_y = 1.0f;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_S)))
        main_y = -1.0f;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
        main_x = -1.0f;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_D)))
        main_x = 1.0f;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_U)))
        secondary_y = 1.0f;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_J)))
        secondary_y = -1.0f;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_H)))
        secondary_x = -1.0f;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_K)))
        secondary_x = 1.0f;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftShift)))
        action_a = true;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_C)))
        action_y = true;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_X)))
        action_x = true;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Z)))
        action_b = true;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_G)))
        action_lb = true;
    if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_L)))
        action_rb = true;


    if (action_y) {
        center_robot();
        m_follow_robot = true;
    }

    if (std::abs(third_x) >= 0.01f || std::abs(third_y) >= 0.01f) {
        m_scene_view.offset_camera(third_x * dt, third_y * dt);
        m_follow_robot = false;
    }

    if (m_robot_ai != nullptr)
        m_robot_ai->set_user_input(main_x, main_y, secondary_x, secondary_y, action_a, action_b, action_x, action_lb, action_rb);
}

void SimulationView::center_robot() {
    if (m_simulation == nullptr)
        return;

    auto robot_position = m_simulation->get_physics_robot()->get_position();
    m_scene_view.move_camera(robot_position.x, robot_position.y);
}
