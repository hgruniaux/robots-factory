#pragma once

#include "part.hpp"

class Shape : public Part {
public:
    // The color of the shape.
    [[nodiscard]] const glm::vec4 &get_color() const { return m_color; }
    void set_color(const glm::vec4 &color) { m_color = color; }

    // The mass of the shape (in kilograms).
    [[nodiscard]] float get_mass() const { return m_mass; }
    void set_mass(float mass) { m_mass = mass; }

    // Computes the center of mass of the shape.
    [[nodiscard]] virtual glm::vec2 get_mass_center() const = 0;

    bool show_inspector() override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

protected:
    glm::vec4 m_color = {1.0f, 1.0f, 1.0f, 1.0f};
    float m_mass = 1.f;
};// class Shape

class CircleShape final : public Shape {
public:
    DECLARE_PART(CircleShape, ICON_FA_CIRCLE);

    // Radius of the circle in meters.
    [[nodiscard]] float get_radius() const { return m_radius; }
    void set_radius(float radius) { m_radius = radius; }

    [[nodiscard]] glm::vec2 get_mass_center() const override;

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

private:
    void check_constraints();

private:
    static constexpr float MIN_RADIUS = 0.0001f;
    static constexpr float MAX_RADIUS = INFINITY;
    float m_radius = 0.5f;
};// class CircleShape

class RectangleShape final : public Shape {
public:
    DECLARE_PART(RectangleShape, ICON_FA_SQUARE);

    // Size of the rectangle in meters.
    [[nodiscard]] glm::vec2 get_size() const { return m_size; }
    void set_size(const glm::vec2 &size) { m_size = size; }

    [[nodiscard]] glm::vec2 get_mass_center() const override;

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

private:
    void check_constraints();

private:
    static constexpr float MIN_SIZE = 0.0001f;
    static constexpr float MAX_SIZE = INFINITY;
    glm::vec2 m_size = {1.0f, 1.0f};
};// class RectangleShape

class PolygonShape final : public Shape {
public:
    DECLARE_PART(PolygonShape, ICON_FA_DRAW_POLYGON);

    [[nodiscard]] glm::vec2 get_mass_center() const override;

    // Vertices of the polygon (coordinates in meters).
    [[nodiscard]] const std::vector<glm::vec2> &get_vertices() const { return m_vertices; }
    void set_vertices(const std::vector<glm::vec2> &vertices) { m_vertices = vertices; }
    void add_vertex(const glm::vec2 &vertex) { m_vertices.push_back(vertex); }

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

private:
    void center_vertices();

private:
    std::vector<glm::vec2> m_vertices;
};// class PolygonShape
