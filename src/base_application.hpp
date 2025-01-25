#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <memory>

class BaseApplication {
public:
    virtual ~BaseApplication() = default;

    bool init();
    void run();
    void shutdown();

protected:
    // Functions to be reimplemented by concrete applications.
    virtual bool start() = 0;
    virtual void stop() = 0;
    virtual void update() = 0;

private:
    GLFWwindow *m_window = nullptr;
};// class Application
