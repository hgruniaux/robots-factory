#include "shape.hpp"

#include "draw/renderer_2d.hpp"
#include "parser_utils.hpp"
#include <imgui.h>

constexpr glm::vec4 OUTLINE_COLOR = {1.0f, 1.0f, 0.0f, 1.0f};

/*
 * The Shape class
 */

bool Shape::show_inspector() {
    bool changed = Part::show_inspector();

    if (ImGui::CollapsingHeader("Shape")) {
        changed |= ImGui::ColorEdit4("Color", &m_color[0]);
        ImGui::SetItemTooltip("The color of the shape. Only used for rendering.");
        changed |= ImGui::DragFloat("Density (in kg/m²)", &m_density, 0.01f, 0.0f, INFINITY);
        ImGui::SetItemTooltip("The density of the shape in kg/m².");
        changed |= ImGui::DragFloat("Friction", &m_friction, 0.01f, 0.0f, INFINITY);
        ImGui::SetItemTooltip("The friction of the shape.");
        changed |= ImGui::DragFloat("Restitution", &m_restitution, 0.01f, 0.0f, 1.0f);
        ImGui::SetItemTooltip("The restitution of the shape.");
        changed |= ImGui::DragFloat("Restitution threshold", &m_restitution_threshold, 0.01f, 0.0f, INFINITY);
        ImGui::SetItemTooltip("The restitution threshold of the shape.");

        check_constraints();
    }

    return changed;
}

void Shape::load(const YAML::Node &node) {
    Part::load(node);

    if (node["color"])
        from_yaml(node["color"], m_color);
    if (node["density"])
        m_density = node["density"].as<float>();
    if (node["friction"])
        m_friction = node["friction"].as<float>();
    if (node["restitution"])
        m_restitution = node["restitution"].as<float>();
    if (node["restitution_threshold"])
        m_restitution_threshold = node["restitution_threshold"].as<float>();

    check_constraints();
}

void Shape::save(YAML::Emitter &emitter) const {
    Part::save(emitter);

    emitter << YAML::Key << "color" << YAML::Value;
    to_yaml(emitter, m_color);
    emitter << YAML::Key << "density" << YAML::Value << m_density;
    emitter << YAML::Key << "friction" << YAML::Value << m_friction;
    emitter << YAML::Key << "restitution" << YAML::Value << m_restitution;
    emitter << YAML::Key << "restitution_threshold" << YAML::Value << m_restitution_threshold;
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
        changed |= ImGui::DragFloat("Radius (in m)", &m_radius, 0.01f, MIN_RADIUS, MAX_RADIUS);
        ImGui::SetItemTooltip("The radius of the circle in meters.");

        check_constraints();
    }

    return changed;
}

void CircleShape::draw(Renderer2D &renderer, const DrawPartContext &context) {
    renderer.set_transform(get_position(), get_angle());

    renderer.draw_solid_circle({0.f, 0.f}, m_radius, m_color);
    if (context.selected_part == this)
        renderer.draw_circle({0.f, 0.f}, m_radius, OUTLINE_COLOR);

    renderer.unset_transform();
}

void CircleShape::load(const YAML::Node &node) {
    Shape::load(node);

    if (node["radius"])
        m_radius = node["radius"].as<float>();

    check_constraints();
}

void CircleShape::save(YAML::Emitter &emitter) const {
    Shape::save(emitter);

    emitter << YAML::Key << "radius" << YAML::Value << m_radius;
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
        changed |= ImGui::DragFloat("Width (in m)", &m_size[0], 0.01f, MIN_SIZE, MAX_SIZE);
        ImGui::SetItemTooltip("The width of the rectangle in meters.");
        changed |= ImGui::DragFloat("Height (in m)", &m_size[1], 0.01f, MIN_SIZE, MAX_SIZE);
        ImGui::SetItemTooltip("The height of the rectangle in meters.");

        check_constraints();
    }

    return changed;
}

void RectangleShape::draw(Renderer2D &renderer, const DrawPartContext &context) {
    renderer.set_transform(get_position(), get_angle());

    renderer.draw_solid_quad({0.f, 0.f}, m_size, m_color);
    if (context.selected_part == this)
        renderer.draw_quad({0.f, 0.f}, m_size, OUTLINE_COLOR);

    renderer.unset_transform();
}

void RectangleShape::load(const YAML::Node &node) {
    Shape::load(node);

    if (node["size"])
        from_yaml(node["size"], m_size);

    check_constraints();
}

void RectangleShape::save(YAML::Emitter &emitter) const {
    Shape::save(emitter);

    emitter << YAML::Key << "size" << YAML::Value;
    to_yaml(emitter, m_size);
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
    renderer.set_transform(get_position(), get_angle());

    renderer.draw_solid_polygon(m_vertices.data(), m_vertices.size(), m_color);
    if (context.selected_part == this)
        renderer.draw_polygon(m_vertices.data(), m_vertices.size(), OUTLINE_COLOR);

    renderer.unset_transform();
}

void PolygonShape::load(const YAML::Node &node) {
    Shape::load(node);

    if (node["vertices"]) {
        m_vertices.clear();
        for (const auto &vertex: node["vertices"]) {
            glm::vec2 v;
            from_yaml(vertex, v);
            m_vertices.push_back(v);
        }
    }
}

void PolygonShape::save(YAML::Emitter &emitter) const {
    Shape::save(emitter);

    emitter << YAML::Key << "vertices" << YAML::Value;
    emitter << YAML::BeginSeq;
    for (const auto &vertex: m_vertices)
        to_yaml(emitter, vertex);
    emitter << YAML::EndSeq;
}

void PolygonShape::center_vertices() {
    glm::vec2 center = {0.f, 0.f};
    for (auto vertex: m_vertices)
        center += vertex;
    center /= (float) m_vertices.size();

    for (auto &vertex: m_vertices)
        vertex -= center;
}
