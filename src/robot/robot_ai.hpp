#pragma once

#include "physics_robot.hpp"
#include "robot.hpp"

class RobotAI {
public:
    struct UserInput {
        float main_x = 0.0f, main_y = 0.0f;
        float secondary_x = 0.0f, secondary_y = 0.0f;
        bool action_a = false;
    };// struct UserInput

    virtual ~RobotAI() = default;

    // Called when the simulation starts and stops.
    virtual void on_start() {}
    virtual void on_stop() {}

    // Called at every simulation step (the AI must be attached to a robot before calling this function).
    virtual void step(float delta_time_in_s) = 0;

    // Called to show the state of the AI in the GUI.
    virtual void show_state() {}

    // Attach the AI to a robot and its physical representation.
    void attach(const std::shared_ptr<Robot> &robot, PhysicsRobot *physics_robot);
    void dettach();

    // User input
    [[nodiscard]] UserInput &get_user_input() { return m_user_input; }
    [[nodiscard]] const UserInput &get_user_input() const { return m_user_input; }
    void set_user_input(float main_x, float main_y, float secondary_x, float secondary_y, bool action_a);

protected:
    [[nodiscard]] std::shared_ptr<Robot> get_robot() const { return m_robot; }
    [[nodiscard]] PhysicsRobot *get_physics_robot() const { return m_physics_robot; }

    UserInput m_user_input;

private:
    std::shared_ptr<Robot> m_robot = nullptr;
    PhysicsRobot *m_physics_robot = nullptr;
};// class RobotAI
