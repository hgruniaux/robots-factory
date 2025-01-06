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
