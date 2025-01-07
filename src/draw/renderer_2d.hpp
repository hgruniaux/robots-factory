#pragma once

#include "shader.hpp"
#include <vector>

class Renderer2D {
public:
    Renderer2D();
    ~Renderer2D();

    [[nodiscard]] unsigned get_texture_id() const { return m_texture; }

    void set_view(const glm::mat4 &view) {
        m_view = view;
        m_projection_view = m_projection * m_view;
    }

    void set_transform(const glm::vec2 &translation, float rotation);
    void set_transform(const glm::vec2 &translation, float rotation, const glm::vec2 &scale);
    void set_transform(const glm::mat4 &transform) { m_transform = transform; }
    void unset_transform() { set_transform(glm::mat4(1.f)); }

    // If enabled, the renderer will draw only the outline of shapes.
    void set_wireframe_mode(bool enabled) { m_wireframe = enabled; }

    // The width and height are in pixels. The scale is the number of pixels per meter.
    void begin(unsigned width, unsigned height, float scale = 300.f);
    void end();

    void draw_point(const glm::vec2 &position, float size, const glm::vec4 &color);
    void draw_line(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color);
    void draw_infinite_line(const glm::vec2 &point, const glm::vec2 &direction, float width, const glm::vec4 &color);
    void draw_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
    void draw_solid_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color);
    void draw_circle(const glm::vec2 &center, float radius, const glm::vec4 &color);
    void draw_solid_circle(const glm::vec2 &center, float radius, const glm::vec4 &color);
    void draw_arc(const glm::vec2 &center, float radius, float arc_start, float arc_end, const glm::vec4 &color);
    void draw_solid_arc(const glm::vec2 &center, float radius, float arc_start, float arc_end, const glm::vec4 &color);
    void draw_polygon(const glm::vec2 *vertices, size_t vertices_count, const glm::vec4 &color);
    void draw_solid_polygon(const glm::vec2 *vertices, size_t vertices_count, const glm::vec4 &color);

    void draw_axes(float size = 1.0f, float subsize = 0.1f);
    void draw_anchor(const glm::vec2 &position, float radius = 0.01f);

    void show_scene_gui();

private:
    void free_fbo();
    void reallocate_fbo(unsigned width, unsigned height);

    struct Vertex2D {
        glm::vec2 position;
        glm::vec4 color;
    };// struct Vertex2D

    void draw_vertices(const Vertex2D *vertices, size_t vertices_count, int mode);

private:
    unsigned m_fbo = 0, m_texture = 0, m_rbo = 0;
    unsigned m_width = 0, m_height = 0;

    unsigned m_vao = 0, m_vbo = 0;
    std::shared_ptr<Shader> m_shader;

    glm::mat4 m_projection, m_view, m_projection_view;
    glm::mat4 m_transform = glm::mat4(1.f);

    // The scale is the number of pixels per meter.
    float m_scale = 0.f;

    // In meters.
    float m_viewport_diag_length = 0.f;
    float m_viewport_width = 0.f, m_viewport_height = 0.f;

    bool m_wireframe = false;
};// class Renderer2D

class SceneView {
public:
    explicit SceneView(Renderer2D &renderer)
        : m_renderer(renderer) {}

    void begin();
    void end();

private:
    static constexpr float DEFAULT_SCALE = 300.f;

    Renderer2D &m_renderer;
    glm::vec2 m_offset = {0.f, 0.f};
    float m_scale = DEFAULT_SCALE;
    bool m_wireframe = false;
};// class SceneView
