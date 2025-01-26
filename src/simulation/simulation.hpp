#pragma once

#include "draw/renderer_2d.hpp"
#include "robot/physics_robot.hpp"
#include "robot/robot.hpp"
#include "robot/robot_ai.hpp"
#include <box2d/box2d.h>
#include <memory>

class Simulation {
public:
    explicit Simulation(const std::shared_ptr<Robot> &robot, const std::shared_ptr<RobotAI> &robot_ai = nullptr, const std::string &world_description = "");
    ~Simulation();

    // The robot and its AI being simulated.
    [[nodiscard]] const std::shared_ptr<Robot> &get_robot() const { return m_robot; }
    [[nodiscard]] const std::shared_ptr<RobotAI> &get_robot_ai() const { return m_robot_ai; }
    [[nodiscard]] const PhysicsRobot *get_physics_robot() const { return m_physics_robot.get(); }

    // The time step of the simulation in seconds.
    [[nodiscard]] float get_time_step() const { return m_time_step; }
    // Total of elapsed simulated time since the start of the simulation in seconds.
    [[nodiscard]] float get_elapsed_time() const { return m_elapsed_time; }

    // The Box2D world being simulated.
    [[nodiscard]] b2World *get_world() const { return m_world.get(); }

    // Step the simulation forward in time (by around 16 ms).
    void step();

    // Draw the simulation.
    void draw(Renderer2D &renderer, bool debug = false);

private:
    void debug_draw(Renderer2D &renderer);
    void normal_draw(Renderer2D &renderer);

private:
    static constexpr glm::vec2 DEFAULT_GRAVITY = {0.0f, -9.80665f};

    std::shared_ptr<RobotAI> m_robot_ai;
    std::shared_ptr<Robot> m_robot;
    std::unique_ptr<b2World> m_world;
    std::unique_ptr<PhysicsRobot> m_physics_robot;
    float m_elapsed_time = 0.0f;

    // Parameters for b2World::Step()
    static constexpr float m_time_step = 1.0f / 60.0f;
    static constexpr int32 m_velocity_iterations = 12;
    static constexpr int32 m_position_iterations = 6;
};// class Simulation
