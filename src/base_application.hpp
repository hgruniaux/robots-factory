#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

class BaseApplication {
public:
    BaseApplication();
    virtual ~BaseApplication() = default;

    static BaseApplication& get() { return *s_instance; }

    bool init();
    void run();
    void shutdown();

    void set_overlay(const glm::vec4& overlay_color);

protected:
    static inline BaseApplication* s_instance = nullptr;

    // Functions to be reimplemented by concrete applications.
    virtual bool start() = 0;
    virtual void stop() = 0;
    virtual void update() = 0;

private:
    GLFWwindow *m_window = nullptr;
    float m_font_size = 0.0f;
    float m_icon_size = 0.0f;

    glm::vec4 m_accent_color = { 0.541f, 0.788f, 0.196f, 1.0f };
    glm::vec4 m_overlay_color = { 0.0f, 0.0f, 0.0f, 0.0f };
    bool m_theme_changed = false;
};// class Application
