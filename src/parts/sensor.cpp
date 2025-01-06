#include "sensor.hpp"
#include "parser_utils.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

/*
 * The AngleSensor class
 */

bool AngleSensor::show_inspector() {
    bool changed = Sensor::show_inspector();

    if (ImGui::CollapsingHeader("Angle sensor")) {
        ImGui::Text("Detection angle (in degrees):");
        changed |= ImGui::DragFloat("Min", &m_min_angle, 1.0f, 0.0f, 360.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        changed |= ImGui::DragFloat("Max", &m_max_angle, 1.0f, m_min_angle, 360.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

        // Enforce constraints if required.
        m_min_angle = std::max(0.0f, std::min(360.0f, m_min_angle));
        m_max_angle = std::max(m_min_angle, m_max_angle);
    }

    return changed;
}

void AngleSensor::load(const YAML::Node &node) {
    Sensor::load(node);

    if (node["min_angle"])
        m_min_angle = node["min_angle"].as<float>();
    if (node["max_angle"])
        m_max_angle = node["max_angle"].as<float>();
}

void AngleSensor::save(YAML::Emitter &emitter) const {
    Sensor::save(emitter);

    emitter << YAML::Key << "min_angle" << YAML::Value << m_min_angle;
    emitter << YAML::Key << "max_angle" << YAML::Value << m_max_angle;
}

/*
 * The Accelerometer class
 */

bool Accelerometer::show_inspector() {
    bool changed = Sensor::show_inspector();

    if (ImGui::CollapsingHeader("Accelerometer")) {
        ImGui::Text("Acceleration (in m/s²):");
        changed |= ImGui::DragFloat2("Min", &m_min_acceleration[0], 0.1f, -INFINITY, INFINITY, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        changed |= ImGui::DragFloat2("Max", &m_max_acceleration[0], 0.1f, -INFINITY, INFINITY, "%.2f", ImGuiSliderFlags_AlwaysClamp);

        // Enforce constraints if required.
        for (int i = 0; i < 2; i++) {
            m_max_acceleration[i] = std::max(m_min_acceleration[i], m_max_acceleration[i]);
        }
    }

    return changed;
}

void Accelerometer::load(const YAML::Node &node) {
    Sensor::load(node);

    if (node["min_acceleration"]) {
        if (node["min_acceleration"].IsScalar()) {
            const auto value = node["min_acceleration"].as<float>();
            m_min_acceleration = {value, value};
        } else {
            from_yaml(node["min_acceleration"], m_min_acceleration);
        }
    }

    if (node["max_acceleration"]) {
        if (node["max_acceleration"].IsScalar()) {
            const auto value = node["max_acceleration"].as<float>();
            m_max_acceleration = {value, value};
        } else {
            from_yaml(node["max_acceleration"], m_max_acceleration);
        }
    }
}

void Accelerometer::save(YAML::Emitter &emitter) const {
    Sensor::save(emitter);

    emitter << YAML::Key << "min_acceleration" << YAML::Value;
    to_yaml(emitter, m_min_acceleration);

    emitter << YAML::Key << "max_acceleration" << YAML::Value;
    to_yaml(emitter, m_max_acceleration);
}

/*
 * The DistanceSensor class
 */

bool DistanceSensor::show_inspector() {
    bool changed = Sensor::show_inspector();

    if (ImGui::CollapsingHeader("Distance sensor")) {
        ImGui::Text("Detection distance (in meters):");
        changed |= ImGui::DragFloat("Min", &m_min_distance, 0.1f, 0.0f, m_max_distance, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        changed |= ImGui::DragFloat("Max", &m_max_distance, 1.0f, m_min_distance, INFINITY, "%.2f", ImGuiSliderFlags_AlwaysClamp);

        // Enforce constraints if required.
        m_min_distance = std::max(0.0f, std::min(m_max_distance, m_min_distance));
        m_max_distance = std::max(m_min_distance, m_max_distance);
    }

    return changed;
}

void DistanceSensor::load(const YAML::Node &node) {
    Sensor::load(node);

    if (node["min_distance"])
        m_min_distance = node["min_distance"].as<float>();
    if (node["max_distance"])
        m_max_distance = node["max_distance"].as<float>();
}

void DistanceSensor::save(YAML::Emitter &emitter) const {
    Sensor::save(emitter);

    emitter << YAML::Key << "min_distance" << YAML::Value << m_min_distance;
    emitter << YAML::Key << "max_distance" << YAML::Value << m_max_distance;
}

/*
 * The ContactSensor class
 */
