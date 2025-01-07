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

void SimulationView::show() {
    // Simulate the simulation by one step
    if (m_simulation != nullptr && !m_simulation_paused) {
        // If in real time, only step
        if (m_real_time) {
            m_time_since_last_step += ImGui::GetIO().DeltaTime * m_time_dilatation;
            if (m_time_since_last_step >= m_simulation->get_time_step()) {
                while (m_time_since_last_step >= m_simulation->get_time_step()) {
                    m_simulation->step();
                    m_time_since_last_step -= m_simulation->get_time_step();
                }

                m_time_since_last_step = 0.0f;
            }
        } else {
            m_simulation->step();
        }
    }

    ImGui::PushID(this);
    if (ImGui::Begin(ICON_FA_FLASK " Simulation")) {
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

void SimulationView::restart_if_needed() {
    if (m_simulation == nullptr)
        return;

    start();
}
