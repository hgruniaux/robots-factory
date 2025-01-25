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

void Part::load(const nlohmann::json& object) {
    m_name = object.value("name", m_name);
    m_position = object.value("position", m_position);
    m_angle = object.value("angle", m_angle);
}

void Part::save(nlohmann::json & object) const {
    object["name"] = m_name;
    object["position"] = m_position;
    object["angle"] = m_angle;
}

void Part::set_name(const std::string &name) {
    if (m_robot != nullptr)
        m_robot->update_part_name(m_name, name);
    m_name = name;
}
