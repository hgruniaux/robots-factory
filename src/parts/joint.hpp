#pragma once

#include "part.hpp"

/**
 * A joint part.
 */
class Joint : public Part {
public:
    // The name of the first part to connect.
    [[nodiscard]] const std::string &get_part_a() const { return m_part_a; }
    void set_part_a(const std::string &part_a) { m_part_a = part_a; }

    // The name of the second part to connect.
    [[nodiscard]] const std::string &get_part_b() const { return m_part_b; }
    void set_part_b(const std::string &part_b) { m_part_b = part_b; }

    // The local anchor point relative to part A center in meters.
    [[nodiscard]] const glm::vec2 &get_local_anchor_a() const { return m_local_anchor_a; }
    void set_local_anchor_a(const glm::vec2 &local_anchor_a) { m_local_anchor_a = local_anchor_a; }

    // The local anchor point relative to part B center in meters.
    [[nodiscard]] const glm::vec2 &get_local_anchor_b() const { return m_local_anchor_b; }
    void set_local_anchor_b(const glm::vec2 &local_anchor_b) { m_local_anchor_b = local_anchor_b; }

    [[nodiscard]] bool should_collide() const { return m_should_collide; }
    void set_should_collide(bool should) { m_should_collide = should; }

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

protected:
    [[nodiscard]] bool should_draw(const DrawPartContext &context) const;

    // The anchor point of part A and B in world coordinates. Only valid if
    // the part exists in the robot.
    [[nodiscard]] glm::vec2 get_world_anchor_a() const;
    [[nodiscard]] glm::vec2 get_world_anchor_b() const;

private:
    std::string m_part_a, m_part_b;
    glm::vec2 m_local_anchor_a, m_local_anchor_b;
    bool m_should_collide;
};// class Joint

/**
 * A distance joint part.
 */
class DistanceJoint : public Joint {
public:
    DECLARE_PART(DistanceJoint, ICON_FA_RULER);

    // The rest length of the joint (in meters).
    [[nodiscard]] float get_length() const { return m_length; }
    void set_length(float length) { m_length = length; }

    // The minimum and maximum rest length of the joint (in meters).
    [[nodiscard]] float get_max_length() const { return m_max_length; }
    [[nodiscard]] float get_min_length() const { return m_min_length; }
    void set_min_length(float min_length) { m_min_length = min_length; }
    void set_max_length(float max_length) { m_max_length = max_length; }

    // The frequency of the joint (in Hz).
    [[nodiscard]] float get_frequency() const { return m_frequency; }
    void set_frequency(float frequency) { m_frequency = frequency; }

    // The damping ratio of the joint.
    [[nodiscard]] float get_damping_ratio() const { return m_damping_ratio; }
    void set_damping_ratio(float damping_ratio) { m_damping_ratio = damping_ratio; }

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

private:
    void ensure_constraints();

private:
    float m_length = 1.0f;
    float m_min_length = 1.0f, m_max_length = 1.0f;
    float m_frequency = 0.f, m_damping_ratio = 1.f;
};// class DistanceJoint

/**
 * A revolute joint part.
 */
class RevoluteJoint : public Joint {
public:
    DECLARE_PART(RevoluteJoint, ICON_FA_ROTATE);

    [[nodiscard]] bool is_limit_enabled() const { return m_limit_enabled; }
    void set_limit_enabled(bool limit_enabled) { m_limit_enabled = limit_enabled; }

    // The lower and upper angle limits in degrees.
    [[nodiscard]] float get_lower_angle() const { return m_lower_angle; }
    [[nodiscard]] float get_upper_angle() const { return m_upper_angle; }
    void set_lower_angle(float lower_angle) { m_lower_angle = lower_angle; }
    void set_upper_angle(float upper_angle) { m_upper_angle = upper_angle; }

    // If the joint is a motor.
    [[nodiscard]] bool is_motor() const { return m_is_motor; }
    void set_motor(bool is_motor) { m_is_motor = is_motor; }

    // The minimum and maximum motor speed in degrees per second.
    [[nodiscard]] float get_min_motor_speed() const { return m_min_motor_speed; }
    [[nodiscard]] float get_max_motor_speed() const { return m_max_motor_speed; }
    void set_min_motor_speed(float min_motor_speed) { m_min_motor_speed = min_motor_speed; }
    void set_max_motor_speed(float max_motor_speed) { m_max_motor_speed = max_motor_speed; }

    // The maximum motor torque in N*m.
    [[nodiscard]] float get_max_motor_torque() const { return m_max_motor_torque; }
    void set_max_motor_torque(float max_motor_torque) { m_max_motor_torque = max_motor_torque; }

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

private:
    void ensure_constraints();

private:
    float m_lower_angle = 0.0f, m_upper_angle = 360.f;
    bool m_limit_enabled = false;
    bool m_is_motor = false;
    float m_min_motor_speed = 0.0f, m_max_motor_speed = 0.0f;
    float m_max_motor_torque = 0.0f;
};// class RevoluteJoint

/**
 * A prismatic joint part.
 */
class PrismaticJoint : public Joint {
public:
    DECLARE_PART(PrismaticJoint, ICON_FA_ARROWS_LEFT_RIGHT);

    [[nodiscard]] const glm::vec2 &get_local_axis_a() const { return m_local_axis_a; }
    void set_local_axis_a(const glm::vec2 &local_axis_a) { m_local_axis_a = local_axis_a; }

    [[nodiscard]] bool is_limit_enabled() const { return m_limit_enabled; }
    void set_limit_enabled(bool limit_enabled) { m_limit_enabled = limit_enabled; }

    // The lower and upper translation limits in meters.
    [[nodiscard]] float get_lower_translation() const { return m_lower_translation; }
    [[nodiscard]] float get_upper_translation() const { return m_upper_translation; }
    void set_lower_translation(float lower_translation) { m_lower_translation = lower_translation; }
    void set_upper_translation(float upper_translation) { m_upper_translation = upper_translation; }

    // If the joint is a motor.
    [[nodiscard]] bool is_motor() const { return m_is_motor; }
    void set_motor(bool is_motor) { m_is_motor = is_motor; }

    // The minimum and maximum motor speed in degrees per second.
    [[nodiscard]] float get_min_motor_speed() const { return m_min_motor_speed; }
    [[nodiscard]] float get_max_motor_speed() const { return m_max_motor_speed; }
    void set_min_motor_speed(float min_motor_speed) { m_min_motor_speed = min_motor_speed; }
    void set_max_motor_speed(float max_motor_speed) { m_max_motor_speed = max_motor_speed; }

    // The maximum motor force in N*m.
    [[nodiscard]] float get_max_motor_force() const { return m_max_motor_force; }
    void set_max_motor_force(float max_motor_force) { m_max_motor_force = max_motor_force; }

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

private:
    void ensure_constraints();

private:
    glm::vec2 m_local_axis_a = {1.0f, 0.0f};
    float m_lower_translation = 0.0f, m_upper_translation = 0.f;
    bool m_limit_enabled = false;
    bool m_is_motor = false;
    float m_min_motor_speed = 0.0f, m_max_motor_speed = 0.0f;
    float m_max_motor_force = 0.0f;
};// class PrismaticJoint
