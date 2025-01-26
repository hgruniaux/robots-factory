#include "part.hpp"

#include <algorithm>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>

#include "draw/renderer_2d.hpp"
#include "parser_utils.hpp"
#include "robot/robot_parser.hpp"
#include "eui.hpp"

/*
 * The Part class
 */

bool Part::show_inspector() {
    bool changed = false;

    std::string old_name = m_name;
    const bool name_changed = EUI::InputText("Name", m_name);
    changed |= name_changed;
    if (name_changed && m_robot != nullptr)
        m_robot->update_part_name(old_name, this);

    if (ImGui::CollapsingHeader("Transform")) {
        changed |= EUI::InputVector("Position", m_position, 0.01f, 0.0f, "%.2f m");
        ImGui::SetItemTooltip("The position of the part in meters.");
        changed |= EUI::InputAngle("Rotation", m_angle, 1.0f);
        ImGui::SetItemTooltip("The rotation of the part in degrees.");
    }

    return changed;
}

void Part::draw(Renderer2D &renderer, const DrawPartContext &context) {
}

void Part::load(const nlohmann::json& object) {
    const auto old_name = m_name;
    m_name = object.value("name", old_name);
    m_position = object.value("position", m_position);
    m_angle = object.value("angle", m_angle);
    m_robot->update_part_name(old_name, this);
}

void Part::save(nlohmann::json & object) const {
    object["name"] = m_name;
    object["position"] = m_position;
    object["angle"] = m_angle;
}

void Part::set_name(const std::string &name) {
    auto old_name = m_name;
    m_name = name;
    if (m_robot != nullptr)
        m_robot->update_part_name(old_name, this);
}

FolderPart *Part::get_folder_parent() const {
    auto* parent = get_parent_part();
    if (parent == nullptr)
        return nullptr;
    return parent->as<FolderPart>();
}

/*
 * FolderPart class
 */

bool FolderPart::show_inspector() {
    bool changed = false;

    std::string old_name = m_name;
    const bool name_changed = EUI::InputText("Name", m_name);
    changed |= name_changed;
    if (name_changed && m_robot != nullptr)
        m_robot->update_part_name(old_name, this);

    changed |= EUI::InputColor("Color", m_color);

    return changed;
}

void FolderPart::draw(Renderer2D &renderer, const DrawPartContext &context) {
    for (const auto &part : m_parts)
        part->draw(renderer, context);
}

void FolderPart::load(const nlohmann::json &object) {
    const auto old_name = m_name;
    m_name = object.value("name", old_name);
    m_color = object.value("color", m_color);
    m_robot->update_part_name(old_name, this);

    clear();

    for (const auto &part_json : object.value("parts", nlohmann::json::array())) {
        auto* part = parse_robot_part(part_json, m_robot);
        add_part(part);
    }
}

void FolderPart::save(nlohmann::json &object) const {
    object["name"] = m_name;
    object["color"] = m_color;

    nlohmann::json parts;
    for (const auto &part : m_parts) {
        nlohmann::json part_object;
        part_object["kind"] = part->get_kind_name();
        part->save(part_object);
        parts.push_back(part_object);
    }
    object["parts"] = parts;
}

void FolderPart::add_part(Part *part) {
    assert(part != nullptr);
    assert(part->m_parent_part == nullptr && "part already has a parent");
    assert(part->m_robot == m_robot && "part must belong to the same robot");

    part->m_parent_part = this;
    m_parts.push_back(part);
}

void FolderPart::insert_part(int index, Part* part) {
    assert(part != nullptr);
    assert(index >= 0 && index <= m_parts.size() && "index out of bounds");
    assert(part->m_parent_part == nullptr && "part already has a parent");
    assert(part->m_robot == m_robot && "part must belong to the same robot");

    part->m_parent_part = this;
    m_parts.insert(m_parts.begin() + index, part);
}

void FolderPart::remove_part(Part *part) {
    assert(part != nullptr);
    assert(part->m_parent_part == this && "part must belong to this folder");

    part->m_parent_part = nullptr;
    auto it = std::find(m_parts.begin(), m_parts.end(), part);
    assert(it != m_parts.end() && "part not found in folder");
    m_parts.erase(it);
}

void FolderPart::clear() {
    for (auto &part : m_parts)
        part->m_parent_part = nullptr;
    m_parts.clear();
}

int FolderPart::get_part_index(Part *part) const {
    assert(part != nullptr);
    auto it = std::find(m_parts.begin(), m_parts.end(), part);
    assert(it != m_parts.end() && "part not found in folder");
    return (int) std::distance(m_parts.begin(), it);
}

void FolderPart::swap_parts(int from_index, int to_index) {
    assert(from_index >= 0 && from_index < m_parts.size());
    assert(to_index >= 0 && to_index < m_parts.size());
    std::swap(m_parts[from_index], m_parts[to_index]);
}
