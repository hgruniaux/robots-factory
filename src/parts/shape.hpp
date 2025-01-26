#pragma once

#include "part.hpp"

class Shape : public Part {
public:
    // The color of the shape.
    [[nodiscard]] const glm::vec4 &get_color() const { return m_color; }
    void set_color(const glm::vec4 &color) { m_color = color; }

    // The density of the shape in kg/m².
    [[nodiscard]] float get_density() const { return m_density; }
    void set_density(float density) { m_density = std::max(density, 0.f); }

    // The friction of the shape.
    [[nodiscard]] float get_friction() const { return m_friction; }
    void set_friction(float friction) { m_friction = std::clamp(friction, 0.f, 1.f); }

    // The restitution of the shape.
    [[nodiscard]] float get_restitution() const { return m_restitution; }
    void set_restitution(float restitution) { m_restitution = std::clamp(restitution, 0.f, 1.f); }

    // The restitution threshold of the shape.
    [[nodiscard]] float get_restitution_threshold() const { return m_restitution_threshold; }
    void set_restitution_threshold(float restitution_threshold) { m_restitution_threshold = std::max(restitution_threshold, 0.f); }

    bool show_inspector() override;
    void load(const nlohmann::json& object) override;
    void save(nlohmann::json& object) const override;

private:
    void check_constraints();

protected:
    glm::vec4 m_color = {1.0f, 1.0f, 1.0f, 1.0f};
    float m_density = 1.f;
    float m_friction = 0.2f;
    float m_restitution = 0.0f;
    float m_restitution_threshold = 1.0f;
};// class Shape

/**
 * A circle shape.
 */
class CircleShape final : public Shape {
public:
    DECLARE_PART(CircleShape, ICON_FA_CIRCLE);

    // Radius of the circle in meters.
    [[nodiscard]] float get_radius() const { return m_radius; }
    void set_radius(float radius) { m_radius = std::clamp(radius, MIN_RADIUS, MAX_RADIUS); }

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const nlohmann::json& object) override;
    void save(nlohmann::json& object) const override;

private:
    void check_constraints();

private:
    static constexpr float MIN_RADIUS = 0.0001f;
    static constexpr float MAX_RADIUS = INFINITY;
    float m_radius = 0.05f;
};// class CircleShape

/**
 * A rectangle shape.
 */
class RectangleShape final : public Shape {
public:
    DECLARE_PART(RectangleShape, ICON_FA_SQUARE);

    // Size of the rectangle in meters.
    [[nodiscard]] glm::vec2 get_size() const { return m_size; }
    void set_size(const glm::vec2 &size) { m_size = glm::clamp(size, MIN_SIZE, MAX_SIZE); }

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const nlohmann::json& object) override;
    void save(nlohmann::json& object) const override;

private:
    void check_constraints();

private:
    static constexpr float MIN_SIZE = 0.0001f;
    static constexpr float MAX_SIZE = INFINITY;
    glm::vec2 m_size = {0.1f, 0.1f};
};// class RectangleShape

/**
 * A polygon shape.
 */
class PolygonShape final : public Shape {
public:
    DECLARE_PART(PolygonShape, ICON_FA_DRAW_POLYGON);

    // Vertices of the polygon (coordinates in meters).
    [[nodiscard]] const std::vector<glm::vec2> &get_vertices() const { return m_vertices; }
    void set_vertices(const std::vector<glm::vec2> &vertices) { m_vertices = vertices; }
    void add_vertex(const glm::vec2 &vertex) { m_vertices.push_back(vertex); }

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const nlohmann::json& object) override;
    void save(nlohmann::json& object) const override;

private:
    void center_vertices();

private:
    std::vector<glm::vec2> m_vertices;
};// class PolygonShape
