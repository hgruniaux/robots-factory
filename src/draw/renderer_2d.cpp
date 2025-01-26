#include "renderer_2d.hpp"

#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <spdlog/spdlog.h>

static constexpr const char *vertex_shader = R"(
#version 330 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec4 a_color;

out vec4 v_color;

uniform mat4 u_mvp;

void main() {
    gl_Position = u_mvp * vec4(a_position, 0.0, 1.0);
    v_color = a_color;
}
)";

static constexpr const char *fragment_shader = R"(
#version 330 core

in vec4 v_color;
out vec4 frag_color;

void main() {
    frag_color = v_color;
}
)";

Renderer2D::Renderer2D() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * 6, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) offsetof(Vertex2D, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *) offsetof(Vertex2D, color));

    m_shader = Shader::create(vertex_shader, fragment_shader);
}

Renderer2D::~Renderer2D() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);

    free_fbo();
}

void Renderer2D::set_transform(const glm::vec2 &translation, float rotation) {
    set_transform(translation, rotation, {1.f, 1.f});
}

void Renderer2D::set_transform(const glm::vec2 &translation, float rotation, const glm::vec2 &scale) {
    m_transform = glm::mat4(1.f);
    m_transform = glm::translate(m_transform, glm::vec3(translation, 0.0f));
    m_transform = glm::rotate(m_transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    m_transform = glm::scale(m_transform, glm::vec3(scale, 1.0f));
}

void Renderer2D::begin(unsigned width, unsigned height, float scale) {
    reallocate_fbo(width, height);

    m_scale = scale;
    const auto w = (float) width;
    const auto h = (float) height;
    m_viewport_diag_length = std::sqrt(w * w + h * h) / scale;
    m_viewport_width = w / scale;
    m_viewport_height = h / scale;

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, (GLsizei) m_width, (GLsizei) m_height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable Blending (for transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable Depth Test (not needed for 2D)
    // glEnable(GL_DEPTH_TEST);

    // Enable Wireframe (for debugging purposes)
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // world centered at (0, 0)
    float left = -(float) width / (2 * scale);
    float right = (float) width / (2 * scale);
    float bottom = (float) height / (2 * scale);
    float top = -(float) height / (2 * scale);
    m_projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_view = glm::mat4(1.f);
    m_projection_view = m_projection * m_view;
}

void Renderer2D::end() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer2D::free_fbo() {
    glDeleteTextures(1, &m_texture);
    glDeleteRenderbuffers(1, &m_rbo);
    glDeleteFramebuffers(1, &m_fbo);
}

void Renderer2D::reallocate_fbo(unsigned int width, unsigned int height) {
    if (m_width == width && m_height == height)
        return;

    free_fbo();

    m_width = width;
    m_height = height;

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Color texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) m_width, (GLsizei) m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    // Depth and stencil renderbuffer
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei) m_width, (GLsizei) m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SPDLOG_ERROR("OpenGL framebuffer is not complete!");
        free_fbo();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer2D::draw_axes(float size, float subsize) {
    constexpr glm::vec4 x_color = {1.0f, 0.0f, 0.0f, 1.0f};
    constexpr glm::vec4 y_color = {0.0f, 1.0f, 0.0f, 1.0f};
    constexpr glm::vec4 grid_color = {0.1f, 0.1f, 0.1f, 1.0f};

    float grid_size = std::pow(10.f, std::floor(std::log10(m_viewport_diag_length / 10.f)));
    const float epsilon = grid_size / 10.f;

    const glm::vec2 viewport_center = glm::inverse(m_projection_view) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Draw y lines (sub grid)
    const float x_start = viewport_center.x - m_viewport_width / 2.f;
    const float x_end = viewport_center.x + m_viewport_width / 2.f;
    for (float x = std::round(x_start / grid_size) * grid_size; x <= x_end + epsilon; x += grid_size) {
        draw_infinite_line({x, 0.f}, {0.f, 1.f}, 0.01f, grid_color);
        draw_infinite_line({-x, 0.f}, {0.f, 1.f}, 0.01f, grid_color);
    }

    // Draw x lines (sub grid)
    const float y_start = viewport_center.y - m_viewport_height / 2.f;
    const float y_end = viewport_center.y + m_viewport_height / 2.f;
    for (float y = std::round(y_start / grid_size) * grid_size; y <= y_end + epsilon; y += grid_size) {
        draw_infinite_line({0.f, y}, {1.f, 0.f}, 0.01f, grid_color);
        draw_infinite_line({0.f, -y}, {1.f, 0.f}, 0.01f, grid_color);
    }

    // Draw XY axes
    draw_infinite_line({0.0f, 0.0f}, {1.0f, 0.0f}, 0.01f, x_color);
    draw_infinite_line({0.0f, 0.0f}, {0.0f, 1.0f}, 0.01f, y_color);
}

void Renderer2D::draw_anchor(const glm::vec2 &position, float radius) {
    constexpr glm::vec4 outer_color = {1.0f, 1.0f, 0.0f, 1.0f};
    constexpr glm::vec4 inner_color = {1.0f, 0.0f, 0.0f, 1.0f};
    draw_solid_circle(position, radius, outer_color);
    draw_solid_circle(position, radius / 2.f, inner_color);
    draw_circle(position, radius, glm::vec4(0.f, 0.f, 0.f, 1.0f));
}

void Renderer2D::draw_point(const glm::vec2 &position, float size, const glm::vec4 &color) {
    draw_quad(position - size / 2.f, {size, size}, color);
}

void Renderer2D::draw_line(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color) {
    Vertex2D vertices[] = {
            {start, color},
            {end, color}};

    glLineWidth(width);
    draw_vertices(vertices, std::size(vertices), GL_LINES);
    glLineWidth(1.0f);
}

void Renderer2D::draw_infinite_line(const glm::vec2 &point, const glm::vec2 &direction, float width, const glm::vec4 &color) {
    glm::vec2 viewport_center = glm::inverse(m_projection_view) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Project viewport_center to the line
    glm::vec2 p = point;
    glm::vec2 d = direction;
    glm::vec2 q = viewport_center;
    glm::vec2 q_p = q - p;
    float t = glm::dot(q_p, d) / glm::dot(d, d);
    glm::vec2 closest_point = p + t * d;

    // Draw the line
    draw_line(closest_point - m_viewport_diag_length * direction, closest_point + m_viewport_diag_length * direction, width, color);
}

static void compute_quad_vertices(glm::vec2 vertices[4], const glm::vec2 &position, const glm::vec2 &size) {
    vertices[0] = position - size / 2.f;
    vertices[1] = position + glm::vec2{size.x, -size.y} / 2.f;
    vertices[2] = position + size / 2.f;
    vertices[3] = position + glm::vec2{-size.x, size.y} / 2.f;
}

void Renderer2D::draw_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
    glm::vec2 vertices[4];
    compute_quad_vertices(vertices, position, size);
    draw_polygon(vertices, std::size(vertices), color);
}

void Renderer2D::draw_solid_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
    glm::vec2 vertices[4];
    compute_quad_vertices(vertices, position, size);
    draw_solid_polygon(vertices, std::size(vertices), color);
}

static std::vector<glm::vec2> compute_circle_vertices(const glm::vec2 &center, float radius) {
    constexpr int samples = (int) (2.f * glm::pi<float>() / 0.1f);

    std::vector<glm::vec2> vertices;
    vertices.reserve(samples + 1);
    for (int i = 0; i <= samples; ++i) {
        float theta = (2.f * glm::pi<float>() * i) / samples;
        glm::vec2 p1 = center + radius * glm::vec2{std::cos(theta), std::sin(theta)};
        vertices.push_back(p1);
    }
    return vertices;
}

void Renderer2D::draw_circle(const glm::vec2 &center, float radius, const glm::vec4 &color) {
    auto vertices = compute_circle_vertices(center, radius);
    draw_polygon((const glm::vec2 *) vertices.data(), vertices.size(), color);
}

void Renderer2D::draw_solid_circle(const glm::vec2 &center, float radius, const glm::vec4 &color) {
    auto vertices = compute_circle_vertices(center, radius);
    draw_solid_polygon((const glm::vec2 *) vertices.data(), vertices.size(), color);
}

static std::vector<glm::vec2> compute_arc_vertices(const glm::vec2 &center, float radius, float arc_start = 0.f, float arc_end = 2.f * glm::pi<float>()) {
    if (arc_start > arc_end)
        std::swap(arc_start, arc_end);

    const int samples = (int) ((arc_end - arc_start) / 0.1f);

    std::vector<glm::vec2> vertices;
    vertices.reserve(samples + 2);
    vertices.push_back(center);
    for (int i = 0; i <= samples; ++i) {
        float theta = ((arc_end - arc_start) * i) / samples + arc_start;
        glm::vec2 p1 = center + radius * glm::vec2{std::cos(theta), std::sin(theta)};
        vertices.push_back(p1);
    }
    return vertices;
}

void Renderer2D::draw_arc(const glm::vec2 &center, float radius, float arc_start, float arc_end, const glm::vec4 &color) {
    auto vertices = compute_arc_vertices(center, radius, arc_start, arc_end);
    draw_polygon((const glm::vec2 *) vertices.data(), vertices.size(), color);
}

void Renderer2D::draw_solid_arc(const glm::vec2 &center, float radius, float arc_start, float arc_end, const glm::vec4 &color) {
    auto vertices = compute_arc_vertices(center, radius, arc_start, arc_end);
    draw_solid_polygon((const glm::vec2 *) vertices.data(), vertices.size(), color);
}

void Renderer2D::draw_cross(const glm::vec2 &position, float size, float width, const glm::vec4 &color) {
    const auto start_1 = position + glm::vec2(-size / 2.f, size / 2.f);
    const auto end_1 = position + glm::vec2(size / 2.f, -size / 2.f);
    const auto start_2 = position + glm::vec2(-size / 2.f, -size / 2.f);
    const auto end_2 = position + glm::vec2(size / 2.f, size / 2.f);
    draw_line(start_1, end_1, width, color);
    draw_line(start_2, end_2, width, color);
}

void Renderer2D::draw_polygon(const glm::vec2 *vertices, size_t vertices_count, const glm::vec4 &color) {
    std::vector<Vertex2D> vertices2d;
    vertices2d.reserve(vertices_count);
    for (size_t i = 0; i < vertices_count; ++i)
        vertices2d.push_back({vertices[i], color});

    draw_vertices(vertices2d.data(), vertices2d.size(), GL_LINE_LOOP);
}

void Renderer2D::draw_solid_polygon(const glm::vec2 *vertices, size_t vertices_count, const glm::vec4 &color) {
    if (m_wireframe)
        return draw_polygon(vertices, vertices_count, color);

    std::vector<Vertex2D> vertices2d;
    vertices2d.reserve(vertices_count);
    for (size_t i = 0; i < vertices_count; ++i)
        vertices2d.push_back({vertices[i], color});

    draw_vertices(vertices2d.data(), vertices2d.size(), GL_TRIANGLE_FAN);
}

void Renderer2D::draw_vertices(const Renderer2D::Vertex2D *vertices, size_t vertices_count, int mode) {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (vertices_count * sizeof(Vertex2D)), vertices, GL_DYNAMIC_DRAW);

    m_shader->use();
    m_shader->set_mat4("u_mvp", m_projection_view * m_transform);

    glBindVertexArray(m_vao);
    glDrawArrays(mode, 0, (GLsizei) vertices_count);
}

/*
 * The SceneView class.
 */

#include "IconsFontAwesome6.h"
#include <imgui.h>

void SceneView::begin() {
    auto image_size = ImGui::GetContentRegionAvail();
    m_renderer.begin((unsigned) image_size.x, (unsigned) image_size.y, m_scale);

    glm::mat4 view = glm::translate(glm::mat4(1.0f), {-m_offset.x, -m_offset.y, 0.0f});
    m_renderer.set_view(view);

    m_renderer.draw_axes();
}

void SceneView::end() {
    auto image_size = ImGui::GetContentRegionAvail();
    m_renderer.end();
    ImGui::Image((void *) (uintptr_t) m_renderer.get_texture_id(), image_size);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
    }

    // Handle zooming and panning.
    if (ImGui::IsItemHovered()) {
        ImGui::SetItemKeyOwner(ImGuiKey_MouseWheelY);
        if (ImGui::GetIO().MouseWheel != 0.0f) {
            const float zoom_factor = 1.1f;// Adjust this factor to control zoom speed
            float scale_change = m_scale * (zoom_factor - 1.0f) * ImGui::GetIO().MouseWheel;
            float new_scale = m_scale + scale_change;
            new_scale = std::max(0.0001f, new_scale);

            // Calculate the cursor position in viewport coordinates before zooming
            const ImVec2 mouse_pos = ImGui::GetMousePos();
            const ImVec2 item_rect_min = ImGui::GetItemRectMin();
            const ImVec2 item_rect_max = ImGui::GetItemRectMax();
            const float item_half_width = (item_rect_max.x - item_rect_min.x) / 2.f;
            const float item_half_height = (item_rect_max.y - item_rect_min.y) / 2.f;
            ImVec2 cursor_pos_in_viewport = {
                    (mouse_pos.x - (item_rect_min.x + item_half_width)) / m_scale,
                    (mouse_pos.y - (item_rect_min.y + item_half_height)) / m_scale};

            m_scale = new_scale;

            // Adjust the offset to preserve the cursor position in viewport coordinates
            m_offset.x += cursor_pos_in_viewport.x * (scale_change / m_scale);
            m_offset.y -= cursor_pos_in_viewport.y * (scale_change / m_scale);
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            m_offset.x -= ImGui::GetIO().MouseDelta.x / m_scale;
            m_offset.y += ImGui::GetIO().MouseDelta.y / m_scale;
        }
    }
}

bool SceneView::is_clicked(int button, glm::vec2 &world_pos) const {
    ImVec2 mouse_pos = ImGui::GetMousePos();
    ImVec2 cursor_screen_pos = ImGui::GetItemRectMin();
    ImVec2 viewport_size = ImGui::GetItemRectSize();
    ImVec2 cursor_pos_in_viewport = {
            (mouse_pos.x - cursor_screen_pos.x),
            (mouse_pos.y - cursor_screen_pos.y)};
    ImVec2 cursor_pos_in_viewport_centered = {
            cursor_pos_in_viewport.x - viewport_size.x / 2.f,
            cursor_pos_in_viewport.y - viewport_size.y / 2.f};

    // Invert the y-axis
    cursor_pos_in_viewport_centered.y = -cursor_pos_in_viewport_centered.y;

    world_pos = {cursor_pos_in_viewport_centered.x / m_scale + m_offset.x, cursor_pos_in_viewport_centered.y / m_scale + m_offset.y};

    if (ImGui::IsItemClicked(button)) {
        return true;
    }

#if 0
    if (ImGui::Begin("Debug")) {
        ImGui::Text("Mouse pos: (%.2f, %.2f)", mouse_pos.x, mouse_pos.y);
        ImGui::Text("Cursor screen pos: (%.2f, %.2f)", cursor_screen_pos.x, cursor_screen_pos.y);
        ImGui::Text("Cursor pos in viewport: (%.2f, %.2f)", cursor_pos_in_viewport.x, cursor_pos_in_viewport.y);
        ImGui::Text("Cursor pos in viewport centered: (%.2f, %.2f)", cursor_pos_in_viewport_centered.x, cursor_pos_in_viewport_centered.y);
        ImGui::Text("World pos: (%.2f, %.2f)", world_pos.x, world_pos.y);
    }
    ImGui::End();
#endif

    return false;
}
