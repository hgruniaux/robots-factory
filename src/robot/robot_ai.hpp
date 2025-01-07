#pragma once

#include "physics_robot.hpp"
#include "robot.hpp"

class RobotAI {
public:
    virtual ~RobotAI() = default;

    // Called when the simulation starts and stops.
    virtual void on_start() {}
    virtual void on_stop() {}

    // Called at every simulation step (the AI must be attached to a robot before calling this function).
    virtual void step(float delta_time_in_s) = 0;

    // Attach the AI to a robot and its physical representation.
    void attach(const std::shared_ptr<Robot> &robot, PhysicsRobot *physics_robot);
    void dettach();

protected:
    [[nodiscard]] std::shared_ptr<Robot> get_robot() const { return m_robot; }
    [[nodiscard]] PhysicsRobot *get_physics_robot() const { return m_physics_robot; }

private:
    std::shared_ptr<Robot> m_robot = nullptr;
    PhysicsRobot *m_physics_robot = nullptr;
};// class RobotAI
