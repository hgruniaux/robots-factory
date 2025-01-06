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
    // Gets the float value of a sensor (e.g. a distance sensor).
    [[nodiscard]] float get_sensor_value(const std::string &name) const;

    // Gets/sets the value of a motor speed. The unit and the range of allowed
    // values depends on the target motor. In all cases, the value is clamped
    // to the allowed range for the target motor.
    [[nodiscard]] float get_motor_speed(const std::string &name) const;
    [[nodiscard]] float get_motor_min_speed(const std::string &name) const;
    [[nodiscard]] float get_motor_max_speed(const std::string &name) const;
    void set_motor_speed(const std::string &name, float value);

    // Retrieve the theoretical representation of the robot.
    [[nodiscard]] Robot &get_robot() { return *m_robot; }
    [[nodiscard]] const Robot &get_robot() const { return *m_robot; }
    [[nodiscard]] Part *get_part_by_name(const std::string &name) const { return m_robot->get_part_by_name(name); }
    template<class T>
    [[nodiscard]] T *get_part_by_name(const std::string &name) const { return m_robot->get_part_by_name<T>(name); }

    // Retrieve the physics representation of the robot.
    [[nodiscard]] PhysicsRobot &get_physics_robot() { return *m_physics_robot; }
    [[nodiscard]] b2Body *get_physics_body(Part *part) const { return m_physics_robot->get_physics_body(part); }
    [[nodiscard]] b2Body *get_physics_body_by_name(const std::string &name) const;
    [[nodiscard]] b2Joint *get_physics_joint(Joint *joint) const { return m_physics_robot->get_physics_joint(joint); }
    [[nodiscard]] b2Joint *get_physics_joint_by_name(const std::string &name) const;

    template<class T>
    [[nodiscard]] T *get_physics_body(Part *part) const { return static_cast<T *>(get_physics_body(part)); }
    template<class T>
    [[nodiscard]] T *get_physics_body_by_name(const std::string &name) const { return static_cast<T *>(get_physics_body_by_name(name)); }

    template<class T>
    [[nodiscard]] T *get_physics_joint(Joint *joint) const { return static_cast<T *>(get_physics_joint(joint)); }
    template<class T>
    [[nodiscard]] T *get_physics_joint_by_name(const std::string &name) const { return static_cast<T *>(get_physics_joint_by_name(name)); }

private:
    std::shared_ptr<Robot> m_robot = nullptr;
    PhysicsRobot *m_physics_robot = nullptr;
};// class RobotAI
