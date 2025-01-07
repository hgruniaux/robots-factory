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
