#pragma once

#include "base_application.hpp"

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

    void load_lua_robot_ai(const std::string &path);
    void load_native_robot_ai(const std::string &path);

private:
    std::shared_ptr<Robot> m_robot;
    std::shared_ptr<RobotAI> m_robot_ai;
    std::unique_ptr<FileWatcher> m_robot_ai_watcher;

    std::unique_ptr<RobotInspectorUI> m_robot_inspector_ui;
    std::unique_ptr<SimulationView> m_simulation_view;

    // Used by the UI.
    bool m_open_new_robot_modal = false;
};// class Application
