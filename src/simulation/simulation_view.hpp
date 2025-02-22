#pragma once

#include "robot/robot.hpp"
#include "simulation.hpp"

class SimulationView {
public:
    void set_robot(const std::shared_ptr<Robot> &robot);
    void set_robot_ai(const std::shared_ptr<RobotAI> &robot_ai);

    void show(bool& should_show);

    [[nodiscard]] bool is_started() const { return m_simulation != nullptr; }
    void start();
    void stop();

    [[nodiscard]] bool is_paused() const { return m_simulation_paused; }
    void pause();
    void resume();

    void step_forward();
    void step_forward_fast(float time_in_s = 1.0f);

private:
    void show_toolbar();
    void show_world();
    void show_robot_info();

    void restart_if_needed();

    void handle_shortcuts();
    void handle_robot_input(float dt);
    void center_robot();

    void update_app_overlay();

private:
    static constexpr glm::vec4 RUN_OVERLAY_COLOR = { 0.541f, 0.788f, 0.196f, 0.15f };
    static constexpr glm::vec4 PAUSE_OVERLAY_COLOR = { 0.541f, 0.788f, 0.196f, 0.05f };

    std::string m_world_description_path;
    std::shared_ptr<Robot> m_robot;
    std::shared_ptr<RobotAI> m_robot_ai;
    Renderer2D m_renderer;
    std::unique_ptr<Simulation> m_simulation;
    SceneView m_scene_view{m_renderer};
    float m_time_since_last_step = 0.0f;
    float m_time_dilatation = 1.0f;
    bool m_simulation_paused = false;
    bool m_real_time = true;
    bool m_follow_robot = false;
    bool m_debug_view = false;
};// class SimulationView
