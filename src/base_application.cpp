#include "base_application.hpp"

#include <GLFW/glfw3.h>
#include <box2d/box2d.h>
#include <glad/glad.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include "robot/robot_parser.hpp"

BaseApplication::BaseApplication() {
    assert(s_instance == nullptr);
    s_instance = this;
}

static void ApplyGrayModernTheme(ImVec4 accent_color = ImVec4(0.35f, 0.35f, 0.9f, 1.00f), ImVec4 overlay_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f)) {
    ImGuiStyle &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;

    auto blend = [overlay_color](const ImVec4 &color) {
        return ImVec4{
                color.x * (1.0f - overlay_color.w) + overlay_color.x * overlay_color.w,
                color.y * (1.0f - overlay_color.w) + overlay_color.y * overlay_color.w,
                color.z * (1.0f - overlay_color.w) + overlay_color.z * overlay_color.w,
                color.w,
        };
    };

    // Base colors
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_CheckMark] = accent_color;
    colors[ImGuiCol_SliderGrab] = accent_color;
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.50f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = accent_color;
    colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_TabSelectedOverline] = accent_color;
    colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.5f);
    colors[ImGuiCol_DockingEmptyBg] = overlay_color;
    colors[ImGuiCol_PlotLines] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(accent_color.x, accent_color.y, accent_color.z, 0.75f);
    colors[ImGuiCol_DragDropTarget] = accent_color;
    colors[ImGuiCol_NavHighlight] = accent_color;
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.60f, 0.60f, 0.60f, 0.60f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    for (int i = 0; i < ImGuiCol_COUNT; ++i)
        colors[i] = blend(colors[i]);
}

static void ApplyModernThemeSpacing() {
    ImGuiStyle &style = ImGui::GetStyle();

    // Style settings
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.WindowRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.ItemSpacing = ImVec2(10.0f, 8.0f);
    style.ScrollbarSize = 12.0f;
    style.FramePadding = ImVec2(8.0f, 4.0f);
}

#ifdef _WIN32
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi")

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif// DWMWA_USE_IMMERSIVE_DARK_MODE

static void enable_dark_title_bar(GLFWwindow *window) {
    // Enable dark title bar on Windows
    HWND hwnd = glfwGetWin32Window(window);
    BOOL enable = TRUE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &enable, sizeof(enable));
}
#endif// _WIN32

bool BaseApplication::init() {
    // Initialize GLFW
    if (!glfwInit()) {
        SPDLOG_CRITICAL("Failed to initialize GLFW");
        return false;
    }

    SPDLOG_INFO("GLFW version {}", glfwGetVersionString());

    // Create window
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, true);
    m_window = glfwCreateWindow(1280, 720, "Robots Factory ;)", nullptr, nullptr);
    if (!m_window) {
        SPDLOG_CRITICAL("Failed to create window");
        shutdown();
        return false;
    }

#ifdef _WIN32
    enable_dark_title_bar(m_window);
#endif// _WIN32

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

    // Get the window content scale (for high-DPI displays)
    float scale_x, scale_y;
    glfwGetWindowContentScale(m_window, &scale_x, &scale_y);

    // Configure the icon font
    m_font_size = 16.0f * scale_y;
    m_icon_size = 14.0f * scale_y;
    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = m_icon_size;// Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
    io.Fonts->AddFontFromFileTTF("fonts/OpenSans-Regular.ttf", m_font_size);
    io.Fonts->AddFontFromFileTTF("fonts/fa-solid-900.ttf", m_icon_size, &config, icon_ranges);

    ImGui::StyleColorsDark();
    ApplyModernThemeSpacing();
    ApplyGrayModernTheme(ImVec4(m_accent_color.x, m_accent_color.y, m_accent_color.z, m_accent_color.w),
                         ImVec4(m_overlay_color.x, m_overlay_color.y, m_overlay_color.z, m_overlay_color.w));

    // Setup ImGui Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    io.DisplayFramebufferScale = {scale_x, scale_y};
    // io.FontGlobalScale = scale_x;
    ImGui::GetStyle().ScaleAllSizes(scale_x);

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

        // Update ImGui theme if required
        if (m_theme_changed) {
            const ImVec4 accentColor = ImVec4(m_accent_color.x, m_accent_color.y, m_accent_color.z, m_accent_color.w);
            const ImVec4 overlayColor = ImVec4(m_overlay_color.x, m_overlay_color.y, m_overlay_color.z, m_overlay_color.w);
            ApplyGrayModernTheme(accentColor, overlayColor);
            m_theme_changed = false;
        }

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
    stop();

    // Shutdown ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Shutdown GLFW
    if (m_window != nullptr)
        glfwDestroyWindow(m_window);
    glfwTerminate();
}

void BaseApplication::set_overlay(const glm::vec4 &overlay_color) {
    m_overlay_color = overlay_color;
    m_theme_changed = true;
}
