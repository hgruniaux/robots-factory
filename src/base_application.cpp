#include "base_application.hpp"

#include <GLFW/glfw3.h>
#include <box2d/box2d.h>
#include <glad/glad.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include "draw/renderer_2d.hpp"
#include "robot/robot_inspector_ui.hpp"
#include "robot/robot_parser.hpp"
#include "simulation/simulation.hpp"
#include "simulation/simulation_view.hpp"

bool BaseApplication::init() {
    // Initialize GLFW
    if (!glfwInit()) {
        SPDLOG_CRITICAL("Failed to initialize GLFW");
        return false;
    }

    SPDLOG_INFO("GLFW version {}", glfwGetVersionString());

    // Create window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, true);
    m_window = glfwCreateWindow(1280, 720, "Hello, World!", nullptr, nullptr);
    if (!m_window) {
        SPDLOG_CRITICAL("Failed to create window");
        shutdown();
        return false;
    }

    // Load OpenGL functions
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        SPDLOG_CRITICAL("Failed to load OpenGL functions");
        shutdown();
        return false;
    }

    SPDLOG_INFO("OpenGL version {}", (const char *) glGetString(GL_VERSION));
    SPDLOG_INFO("OpenGL renderer: {}", (const char *) glGetString(GL_RENDERER));
    SPDLOG_INFO("OpenGL vendor: {}", (const char *) glGetString(GL_VENDOR));

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls

    // Configure the icon font
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f;// Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup ImGui Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float scale_x, scale_y;
    glfwGetWindowContentScale(m_window, &scale_x, &scale_y);
    io.DisplayFramebufferScale = {scale_x, scale_y};
    io.FontGlobalScale = scale_x;
    style.ScaleAllSizes(scale_x);

    SPDLOG_INFO("Box2D version {}.{}.{}", b2_version.major, b2_version.minor, b2_version.revision);

    if (!start()) {
        SPDLOG_CRITICAL("Failed to start application");
        shutdown();
        return false;
    }

    return true;
}

void BaseApplication::run() {
    ImGuiIO &io = ImGui::GetIO();

    while (!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render ImGui
        ImGui::DockSpaceOverViewport();

        update();

        // Render ImGui
        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // Swap buffers
        glfwSwapBuffers(m_window);
    }
}

void BaseApplication::shutdown() {
    // Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Shutdown GLFW
    if (m_window != nullptr)
        glfwDestroyWindow(m_window);
    glfwTerminate();
}
