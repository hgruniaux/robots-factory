#include "sensor.hpp"
#include "draw/renderer_2d.hpp"
#include "parser_utils.hpp"
#include "robot/robot.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

/*
 * The Sensor class
 */

bool Sensor::should_draw(const DrawPartContext &context) const {
    if (context.selected_part != this || !context.draw_joints)
        return false;

    auto *part = get_robot()->get_part_by_name(m_part);
    if (part == nullptr)
        return false;

    return true;
}

glm::vec2 Sensor::get_world_anchor() const {
    auto *part = get_robot()->get_part_by_name(m_part);
    assert(part != nullptr);

    const float angle = glm::radians(part->get_angle());
    const float sin_a = std::sin(angle), cos_a = std::cos(angle);
    glm::vec2 world_anchor = part->get_position();
    world_anchor += glm::vec2(cos_a * m_local_anchor.x - sin_a * m_local_anchor.y,
                              sin_a * m_local_anchor.x + cos_a * m_local_anchor.y);
    return world_anchor;
}

bool Sensor::show_inspector() {
    bool changed = Part::show_inspector();

    if (ImGui::CollapsingHeader("Sensor")) {
        changed |= ImGui::InputText("Part", &m_part);
        ImGui::SetItemTooltip("The name of the part to connect.");

        const bool part_exists = get_robot()->get_part_by_name(m_part) != nullptr;
        if (!part_exists)
            ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, ICON_FA_TRIANGLE_EXCLAMATION " Part does not exist!");

        changed |= ImGui::DragFloat2("Local anchor", &m_local_anchor.x, 0.01f);
        ImGui::SetItemTooltip("The local anchor point relative to the part center in meters.");
    }

    return changed;
}

void Sensor::draw(Renderer2D &renderer, const DrawPartContext &context) {
    if (!should_draw(context))
        return;

    // Draw the anchor point.
    renderer.draw_anchor(get_world_anchor());
}

void Sensor::load(const nlohmann::json &object) {
    Part::load(object);

    m_part = object.value("part", m_part);
    m_local_anchor = object.value("local_anchor", m_local_anchor);
}

void Sensor::save(nlohmann::json &object) const {
    Part::save(object);

    object["part"] = m_part;
    object["local_anchor"] = m_local_anchor;
}

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

void AngleSensor::load(const nlohmann::json &object) {
    Sensor::load(object);

    m_min_angle = object.value("min_angle", m_min_angle);
    m_max_angle = object.value("max_angle", m_max_angle);
}

void AngleSensor::save(nlohmann::json &object) const {
    Sensor::save(object);

    object["min_angle"] = m_min_angle;
    object["max_angle"] = m_max_angle;
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

        check_constraints();
    }

    return changed;
}

void DistanceSensor::draw(Renderer2D &renderer, const DrawPartContext &context) {
    Sensor::draw(renderer, context);

    if (!should_draw(context))
        return;

    auto *part = get_robot()->get_part_by_name(get_part());
    assert(part != nullptr);

    // Draw the direction
    const glm::vec2 world_anchor = get_world_anchor();
    const float angle = glm::radians(part->get_angle() + get_angle());
    const glm::vec2 direction = glm::vec2(std::cos(angle), std::sin(angle));

    constexpr glm::vec4 marker_color = {1.0f, 1.0f, 0.0f, 1.0f};
    constexpr float marker_width = 0.01f;

    float max_distance = m_max_distance;
    if (max_distance == INFINITY)
        max_distance = 100000.f;// to be sure it's far enough
    renderer.draw_line(world_anchor + direction * m_min_distance, world_anchor + direction * max_distance, marker_width, marker_color);
}

void DistanceSensor::load(const nlohmann::json &object) {
    Sensor::load(object);

    m_min_distance = object.value("min_distance", m_min_distance);
    m_max_distance = object.value("max_distance", m_max_distance);
    if (m_max_distance >= 1e30f) // JSON doesn't support inf
        m_max_distance = INFINITY;

    check_constraints();
}

void DistanceSensor::save(nlohmann::json &object) const {
    Sensor::save(object);

    object["min_distance"] = m_min_distance;
    if (m_max_distance == INFINITY)
        object["max_distance"] = std::numeric_limits<float>::max();
    else
        object["max_distance"] = m_max_distance;
}

void DistanceSensor::check_constraints() {
    m_min_distance = std::max(0.0f, std::min(m_max_distance, m_min_distance));
    m_max_distance = std::max(m_min_distance, m_max_distance);
}
