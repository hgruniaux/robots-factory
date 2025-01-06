#include "part.hpp"

#include <algorithm>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>

#include "draw/renderer_2d.hpp"
#include "parser_utils.hpp"
#include "robot/robot.hpp"

/*
 * The Part class
 */

bool Part::show_inspector() {
    bool changed = false;

    std::string old_name = m_name;
    const bool name_changed = ImGui::InputText("Name", &m_name);
    changed |= name_changed;
    if (name_changed && m_robot != nullptr)
        m_robot->update_part_name(old_name, m_name);

    if (ImGui::CollapsingHeader("Transform")) {
        changed |= ImGui::DragFloat2("Position", &m_position[0], 0.01f);
        ImGui::SetItemTooltip("The position of the part in meters.");
        changed |= ImGui::DragFloat("Rotation", &m_angle, 1.0f);
        ImGui::SetItemTooltip("The rotation of the part in degrees.");

        if (m_angle < 0.0f)
            m_angle += 360.f;
        m_angle = std::fmod(m_angle, 360.f);
    }

    return changed;
}

void Part::draw(Renderer2D &renderer, const DrawPartContext &context) {
}

void Part::load(const YAML::Node &node) {
    if (node["name"])
        m_name = node["name"].as<std::string>();
    if (node["position"])
        from_yaml(node["position"], m_position);
    if (node["angle"])
        m_angle = node["angle"].as<float>();
}

void Part::save(YAML::Emitter &emitter) const {
    emitter << YAML::Key << "name" << YAML::Value << m_name;
    emitter << YAML::Key << "position" << YAML::Value;
    to_yaml(emitter, m_position);
    emitter << YAML::Key << "angle" << YAML::Value << m_angle;
}

void Part::set_name(const std::string &name) {
    if (m_robot != nullptr)
        m_robot->update_part_name(m_name, name);
    m_name = name;
}

/*
 * The LED class
 */

bool Led::show_inspector() {
    bool changed = Part::show_inspector();

    if (ImGui::CollapsingHeader("LED")) {
        changed |= ImGui::Checkbox("Is on?", &m_state);
        changed |= ImGui::ColorEdit3("Color", &m_color[0]);
    }

    return changed;
}

void Led::draw(Renderer2D &renderer, const DrawPartContext &context) {
    renderer.set_transform(get_position(), get_angle());
    if (m_state) {
        renderer.draw_solid_circle({0.0f, 0.0f}, 0.1f, glm::vec4(m_color, 1.0f));
    } else {
        renderer.draw_solid_circle({0.0f, 0.0f}, 0.1f, {0.1f, 0.1f, 0.1f, 1.0f});
    }
    renderer.unset_transform();
}

void Led::load(const YAML::Node &node) {
    Part::load(node);
    if (node["color"])
        from_yaml(node["color"], m_color);
    if (node["state"])
        m_state = node["state"].as<bool>();
}

void Led::save(YAML::Emitter &emitter) const {
    Part::save(emitter);
    emitter << YAML::Key << "color" << YAML::Value;
    to_yaml(emitter, m_color);
    emitter << YAML::Key << "state" << YAML::Value << m_state;
}
