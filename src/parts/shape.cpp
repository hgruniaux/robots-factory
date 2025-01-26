#include "shape.hpp"

#include "draw/renderer_2d.hpp"
#include "parser_utils.hpp"
#include "eui.hpp"

constexpr glm::vec4 OUTLINE_COLOR = {1.0f, 1.0f, 0.0f, 1.0f};
constexpr glm::vec4 OVERLAY_COLOR = {1.0f, 1.0f, 0.0f, 0.2f};

/*
 * The Shape class
 */

bool Shape::show_inspector() {
    bool changed = Part::show_inspector();

    if (ImGui::CollapsingHeader("Shape")) {
        changed |= EUI::InputColor("Color", m_color);
        ImGui::SetItemTooltip("The color of the shape. Only used for rendering.");
        changed |= EUI::InputFloat("Density", m_density, 0.01f, 0.f, INFINITY, "%.2f kg/m²");
        ImGui::SetItemTooltip("The density of the shape in kg/m².");
        changed |= EUI::InputFloat("Friction", m_friction, 0.01f, 0.f, INFINITY);
        ImGui::SetItemTooltip("The friction of the shape.");
        changed |= EUI::InputFloat("Restitution", m_restitution, 0.01f, 0.f, 1.f);
        ImGui::SetItemTooltip("The restitution of the shape.");
        changed |= EUI::InputFloat("Restitution threshold", m_restitution_threshold, 0.01f, 0.f, INFINITY);
        ImGui::SetItemTooltip("The restitution threshold of the shape.");

        check_constraints();
    }

    return changed;
}

void Shape::load(const nlohmann::json& object) {
    Part::load(object);

    m_color = object.value("color", m_color);
    m_density = object.value("density", m_density);
    m_friction = object.value("friction", m_friction);
    m_restitution = object.value("restitution", m_restitution);
    m_restitution_threshold = object.value("restitution_threshold", m_restitution_threshold);

    check_constraints();
}

void Shape::save(nlohmann::json& object) const {
    Part::save(object);

    object["color"] = m_color;
    object["density"] = m_density;
    object["friction"] = m_friction;
    object["restitution"] = m_restitution;
    object["restitution_threshold"] = m_restitution_threshold;
}

void Shape::check_constraints() {
    m_density = std::max(m_density, 0.f);
    m_friction = std::max(m_friction, 0.f);
    m_restitution = std::clamp(m_restitution, 0.f, 1.f);
    m_restitution_threshold = std::max(m_restitution_threshold, 0.f);
}

/*
 * The CircleShape class
 */

bool CircleShape::show_inspector() {
    bool changed = Shape::show_inspector();

    if (ImGui::CollapsingHeader("Circle shape")) {
        changed |= EUI::InputFloat("Radius", m_radius, 0.01f, MIN_RADIUS, MAX_RADIUS, "%.2f m");
        ImGui::SetItemTooltip("The radius of the circle in meters.");

        check_constraints();
    }

    return changed;
}

void CircleShape::draw(Renderer2D &renderer, const DrawPartContext &context) {
    draw(renderer, context, get_position(), get_angle());
}

void CircleShape::draw(Renderer2D &renderer, const DrawPartContext &context, const glm::vec2& position, float angle) {
    renderer.set_transform(position, angle);

    renderer.draw_solid_circle({0.f, 0.f}, m_radius, m_color);
    if (context.selected_part == this) {
        renderer.draw_solid_circle({0.f, 0.f}, m_radius, OVERLAY_COLOR);
        renderer.draw_circle({0.f, 0.f}, m_radius, OUTLINE_COLOR);
    }

    renderer.unset_transform();
}

void CircleShape::load(const nlohmann::json& object) {
    Shape::load(object);

    m_radius = object.value("radius", m_radius);

    check_constraints();
}

void CircleShape::save(nlohmann::json& object) const {
    Shape::save(object);

    object["radius"] = m_radius;
}

void CircleShape::check_constraints() {
    m_radius = std::clamp(m_radius, MIN_RADIUS, MAX_RADIUS);
}

/*
 * The RectangleShape class
 */

bool RectangleShape::show_inspector() {
    bool changed = Shape::show_inspector();

    if (ImGui::CollapsingHeader("Rectangle shape")) {
        changed |= EUI::InputVector("Size", m_size, 0.01f, 0.1f, "%.2f m");

        check_constraints();
    }

    return changed;
}

void RectangleShape::draw(Renderer2D &renderer, const DrawPartContext &context) {
    draw(renderer, context, get_position(), get_angle());
}

void RectangleShape::draw(Renderer2D &renderer, const DrawPartContext &context, const glm::vec2& position, float angle) {
    renderer.set_transform(position, angle);

    renderer.draw_solid_quad({0.f, 0.f}, m_size, m_color);
    if (context.selected_part == this) {
        renderer.draw_solid_quad({0.f, 0.f}, m_size, OVERLAY_COLOR);
        renderer.draw_quad({0.f, 0.f}, m_size, OUTLINE_COLOR);
    }

    renderer.unset_transform();
}

void RectangleShape::load(const nlohmann::json& object) {
    Shape::load(object);

    m_size = object.value("size", m_size);

    check_constraints();
}

void RectangleShape::save(nlohmann::json& object) const {
    Shape::save(object);

    object["size"] = m_size;
}

void RectangleShape::check_constraints() {
    m_size = glm::clamp(m_size, glm::vec2{MIN_SIZE, MIN_SIZE}, glm::vec2{MAX_SIZE, MAX_SIZE});
}

/*
 * The PolygonShape class
 */

bool PolygonShape::show_inspector() {
    bool changed = Shape::show_inspector();

    if (ImGui::CollapsingHeader("Polygon shape")) {
        if (ImGui::Button(ICON_FA_ARROWS_TO_DOT " Center vertices")) {
            center_vertices();
            changed = true;
        }
        ImGui::SetItemTooltip("Center the vertices around the mass center.");

        int vertex_to_remove = -1;
        ImGui::Text("Vertices:");
        for (int i = 0; i < m_vertices.size(); i++) {
            ImGui::PushID((int) i);
            changed |= ImGui::DragFloat2("##vertex", &m_vertices[i][0], 0.01f, -INFINITY, INFINITY, "%.2f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_MINUS "##remove"))
                vertex_to_remove = i;

            ImGui::PopID();
        }

        if (vertex_to_remove != -1) {
            m_vertices.erase(m_vertices.begin() + vertex_to_remove);
            changed = true;
        }


        if (ImGui::Button(ICON_FA_PLUS " Add vertex")) {
            m_vertices.push_back({0.0f, 0.0f});
            changed = true;
        }
    }

    return changed;
}

void PolygonShape::draw(Renderer2D &renderer, const DrawPartContext &context) {
    draw(renderer, context, get_position(), get_angle());
}

void PolygonShape::draw(Renderer2D &renderer, const DrawPartContext &context, const glm::vec2& position, float angle) {
    renderer.set_transform(position, angle);

    renderer.draw_solid_polygon(m_vertices.data(), m_vertices.size(), m_color);
    if (context.selected_part == this) {
        renderer.draw_solid_polygon(m_vertices.data(), m_vertices.size(), OVERLAY_COLOR);
        renderer.draw_polygon(m_vertices.data(), m_vertices.size(), OUTLINE_COLOR);
    }

    renderer.unset_transform();
}

void PolygonShape::load(const nlohmann::json& object) {
    Shape::load(object);

    if (object.contains("vertices")) {
        m_vertices.clear();
        for (const auto &vertex: object["vertices"]) {
            glm::vec2 v;
            from_json(vertex, v);
            m_vertices.push_back(v);
        }
    }
}

void PolygonShape::save(nlohmann::json& object) const {
    Shape::save(object);

    auto vertices = nlohmann::json::array();
    for (const auto &vertex: m_vertices)
        vertices.push_back(vertex);
    object["vertices"] = vertices;
}

void PolygonShape::center_vertices() {
    glm::vec2 center = {0.f, 0.f};
    for (auto vertex: m_vertices)
        center += vertex;
    center /= (float) m_vertices.size();

    for (auto &vertex: m_vertices)
        vertex -= center;
}
