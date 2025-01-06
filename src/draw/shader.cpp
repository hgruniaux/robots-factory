#include "shader.hpp"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

Shader::Shader(unsigned int id) : m_id(id) {
    assert(glIsProgram(m_id));
}

GLuint compile_shader(GLenum type, const char *code) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
        SPDLOG_ERROR("Failed to compile shader: {}", info_log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

std::shared_ptr<Shader> Shader::create(const char *vertex_shader_code, const char *fragment_shader_code) {
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_code);
    if (!vertex_shader)
        return nullptr;

    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_code);
    if (!fragment_shader) {
        glDeleteShader(vertex_shader);
        return nullptr;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program, sizeof(info_log), nullptr, info_log);
        SPDLOG_ERROR("Failed to link program: {}", info_log);
        glDeleteProgram(program);
        return nullptr;
    }

    return std::make_shared<Shader>(program);
}

Shader::~Shader() {
    glDeleteProgram(m_id);
}

void Shader::use() {
    glUseProgram(m_id);
}

void Shader::set_bool(const char *name, bool value) {
    glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::set_int(const char *name, int value) {
    glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::set_float(const char *name, float value) {
    glUniform1f(glGetUniformLocation(m_id, name), value);
}

void Shader::set_vec2(const char *name, const glm::vec2 &value) {
    glUniform2fv(glGetUniformLocation(m_id, name), 1, &value[0]);
}

void Shader::set_vec3(const char *name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(m_id, name), 1, &value[0]);
}

void Shader::set_vec4(const char *name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(m_id, name), 1, &value[0]);
}

void Shader::set_mat3(const char *name, const glm::mat3 &value) {
    glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::set_mat4(const char *name, const glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, &value[0][0]);
}
