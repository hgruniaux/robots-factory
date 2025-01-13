#include "robot_ai.hpp"
#include "parts/joint.hpp"
#include "parts/sensor.hpp"

void RobotAI::attach(const std::shared_ptr<Robot> &robot, PhysicsRobot *physics_robot) {
    assert(robot != nullptr);
    assert(physics_robot != nullptr);

    m_robot = robot;
    m_physics_robot = physics_robot;
    on_start();
}

void RobotAI::dettach() {
    on_stop();
    m_robot = nullptr;
    m_physics_robot = nullptr;
}

void RobotAI::set_user_input(float main_x, float main_y, float secondary_x, float secondary_y, bool action_a, bool action_b, bool action_x, bool action_lb, bool action_rb) {
    m_user_input.main_x = main_x;
    m_user_input.main_y = main_y;
    m_user_input.secondary_x = secondary_x;
    m_user_input.secondary_y = secondary_y;
    m_user_input.action_a = action_a;
    m_user_input.action_b = action_b;
    m_user_input.action_x = action_x;
    m_user_input.action_lb = action_lb;
    m_user_input.action_rb = action_rb;
}
