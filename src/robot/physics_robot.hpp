#pragma once

#include "robot.hpp"

#include <box2d/box2d.h>
#include <unordered_map>

class Part;
class Shape;
class Joint;
class Sensor;

/**
 * The physics representation (in Box2D) of a robot.
 *
 * This class is responsible for creating the Box2D bodies and joints that represent the robot parts.
 * And to synchronize both representation if required.
 */
class PhysicsRobot {
public:
    PhysicsRobot(const std::shared_ptr<Robot> &robot, b2World *world, b2Body *ground_body, bool lax = true);
    PhysicsRobot(const std::shared_ptr<Robot> &robot, const glm::vec2 &position, float angle, b2World *world, b2Body *ground_body, bool lax = true);

    void sync_to_box2d();
    void sync_from_box2d();

    // Retrieve the robot being simulated.
    [[nodiscard]] const std::shared_ptr<Robot> &get_robot() const { return m_robot; }

    // Retrieve the Box2D world where the robot is simulated.
    [[nodiscard]] b2World *get_world() const { return m_world; }

    // Retrieve the world space position of the robot.
    [[nodiscard]] glm::vec2 get_position() const;

    // Retrieve the Box2D representation of a part or joint.
    [[nodiscard]] b2Body *get_physics_body(Part *part) const;
    [[nodiscard]] b2Joint *get_physics_joint(Joint *joint) const;
    [[nodiscard]] b2Body *get_physics_body_by_name(const std::string &name) const;
    [[nodiscard]] b2Joint *get_physics_joint_by_name(const std::string &name) const;

    // Checks for collisions on parts of the robot.
    [[nodiscard]] bool has_collision() const;
    [[nodiscard]] bool has_collision(Part *part) const;
    [[nodiscard]] bool has_collision_by_name(const std::string &name) const;

    // Retrieve the sensor value of a joint.
    [[nodiscard]] float get_sensor_value(const std::string &name) const;

    // Enable/disable the motors of the robot.
    [[nodiscard]] bool is_motor_enabled(const std::string &name) const;
    void set_motor_enabled(const std::string &name, bool enabled);

    // Access and modify the motors speed of the robot.
    [[nodiscard]] float get_motor_speed(const std::string &name) const;
    [[nodiscard]] float get_motor_min_speed(const std::string &name) const { return get_motor_speed_range(name).x; }
    [[nodiscard]] float get_motor_max_speed(const std::string &name) const { return get_motor_speed_range(name).y; }
    [[nodiscard]] glm::vec2 get_motor_speed_range(const std::string &name) const;
    [[nodiscard]] float get_motor_max_force(const std::string &name) const;
    void set_motor_speed(const std::string &name, float value);

private:
    [[nodiscard]] float get_sensor_value(Sensor *sensor) const;
    [[nodiscard]] float get_sensor_value(Joint *joint) const;

private:
    void clear();

    struct BuildContext {
        // Initial position and angle of the robot.
        glm::vec2 position = {0.f, 0.f};// in meters
        float angle = 0.f;              // in degrees

        // If true, we ignore invalid parts when building the physics representation
        // (like joints referencing unknown parts, etc.).
        bool lax = false;
    };// struct BuildContext

    // Build the physics representation.
    void build(const BuildContext &ctx);
    void build_part(const BuildContext &ctx, Part *part);
    void build_shape_part(const BuildContext &ctx, Shape *part);
    void build_joint_part(const BuildContext &ctx, Joint *part);

private:
    // The robot to be simulated.
    std::shared_ptr<Robot> m_robot;

    // The Box2D world where the robot is simulated.
    b2World *m_world = nullptr;
    // The Box2D body that should be used as the ground (to resolve the symbolic @ground@ part name).
    b2Body *m_ground_body = nullptr;

    // The Box2D main body of the robot (used to retrieve the global robot center location).
    b2Body *m_main_body = nullptr;

    // Mapping between the robot parts and their Box2D physics representation.
    std::unordered_map<Part *, b2Body *> m_bodies;
    std::unordered_map<Joint *, b2Joint *> m_joints;

    // If true, we add noise to the motor speed and sensor values
    // (to emulate real world imperfection).
    bool m_noisy = false;
};// class PhysicsRobot
