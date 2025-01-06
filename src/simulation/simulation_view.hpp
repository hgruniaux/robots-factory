#pragma once

#include "robot/robot.hpp"
#include "simulation.hpp"

class SimulationView {
public:
    void set_robot(const std::shared_ptr<Robot> &robot);
    void set_robot_ai(const std::shared_ptr<RobotAI> &robot_ai);

    void show();

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

    void restart_if_needed();

private:
    std::shared_ptr<Robot> m_robot;
    std::shared_ptr<RobotAI> m_robot_ai;
    Renderer2D m_renderer;
    std::unique_ptr<Simulation> m_simulation;
    SceneView m_scene_view{m_renderer};
    bool m_simulation_paused = false;
    bool m_real_time = true;
    float m_time_since_last_step = 0.0f;
};// class SimulationView
