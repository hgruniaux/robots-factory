#pragma once

#include "part.hpp"

/**
 * A sensor is a node that detects a physical quantity (like a contact or a distance).
 */
class Sensor : public Part {
public:
};// class Sensor

/**
 * A sensor that detects a global angle (relative to the world, like a gyroscope).
 */
class AngleSensor final : public Sensor {
public:
    DECLARE_PART(AngleSensor, ICON_FA_ARROWS_SPIN);

    // The minimum and maximum angle (in degrees) the sensor can detect.
    // The default values are 0.0 and 360.0.
    [[nodiscard]] float get_min_angle() const { return m_min_angle; }
    [[nodiscard]] float get_max_angle() const { return m_max_angle; }
    void set_min_angle(float min_angle) { m_min_angle = min_angle; }
    void set_max_angle(float max_angle) { m_max_angle = max_angle; }

    bool show_inspector() override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

private:
    // The minimum and maximum angle (in radians) the sensor can detect.
    float m_min_angle = 0.0f;
    float m_max_angle = 360.0f;
};// class AngleSensor

/**
 * A sensor that detects distance in a given direction (like a IR distance sensor).
 */
class DistanceSensor final : public Sensor {
public:
    DECLARE_PART(DistanceSensor, ICON_FA_RULER);

    // The minimum and maximum distance (in meters) the sensor can detect.
    // The distance is measured from the sensor to the detected object.
    // The default values are 0.0 and INFINITY.
    [[nodiscard]] float get_min_distance() const { return m_min_distance; }
    [[nodiscard]] float get_max_distance() const { return m_max_distance; }
    void set_min_distance(float min_distance) { m_min_distance = min_distance; }
    void set_max_distance(float max_distance) { m_max_distance = max_distance; }

    bool show_inspector() override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

private:
    float m_min_distance = 0.0f;
    float m_max_distance = INFINITY;
};// class DistanceSensor

/**
 * A sensor that detects contact (like a touch sensor).
 */
class ContactSensor final : public Sensor {
public:
    DECLARE_PART(ContactSensor, ICON_FA_HAND_POINTER);
};// class ContactSensor
