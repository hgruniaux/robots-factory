#pragma once

#include "base_application.hpp"
#include "shared_library.hpp"

class RobotAI;
class RobotInspectorUI;
class Robot;
class SimulationView;
class FileWatcher;

class Application final : public BaseApplication {
public:
    Application();
    ~Application() override;

protected:
    bool start() override;
    void stop() override;
    void update() override;

private:
    void show_main_menu_bar();
    void show_new_robot_modal();
    void show_robot_ai_inspector();

    void new_robot();

    void load_robot();
    void load_robot(const std::string &path);
    void save_robot();
    void save_robot(const std::string &path);
    void save_robot_as();

    void load_robot_ai();
    void load_robot_ai(const std::string &path);
    void load_native_robot_ai(const std::string &path);

    void reload_robot_ai();
    void unload_robot_ai();

    void load_settings();
    void save_settings();

private:
    std::shared_ptr<Robot> m_robot;
    std::shared_ptr<RobotAI> m_robot_ai;
    std::unique_ptr<SharedLibrary> m_robot_ai_lib;
    std::string m_robot_ai_path;

    std::unique_ptr<RobotInspectorUI> m_robot_inspector_ui;
    std::unique_ptr<SimulationView> m_simulation_view;

    // Used by the UI.
    bool m_open_new_robot_modal = false;

    // UI windows.
    bool m_show_robot_inspector = true;
    bool m_show_robot_part_inspector = true;
    bool m_show_robot_preview = true;
    bool m_show_robot_ai_inspector = true;
    bool m_show_simulation_view = true;
};// class Application
