#pragma once

#include <glm/glm.hpp>
#include <memory>

class Shader {
public:
    explicit Shader(unsigned id);
    ~Shader();

    static std::shared_ptr<Shader> create(const char *vertex_shader, const char *fragment_shader);

    void use();
    void set_bool(const char *name, bool value);
    void set_int(const char *name, int value);
    void set_float(const char *name, float value);
    void set_vec2(const char *name, const glm::vec2 &value);
    void set_vec3(const char *name, const glm::vec3 &value);
    void set_vec4(const char *name, const glm::vec4 &value);
    void set_mat3(const char *name, const glm::mat3 &value);
    void set_mat4(const char *name, const glm::mat4 &value);

private:
    unsigned m_id;
};// class Shader
