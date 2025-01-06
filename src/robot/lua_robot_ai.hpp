#pragma once

#include "robot_ai.hpp"

struct lua_State;

/**
 * A robot AI that is controlled by a Lua script.
 */
class LuaRobotAI final : public RobotAI {
public:
    LuaRobotAI();
    ~LuaRobotAI() override;

    bool load_from_string(std::string_view code);
    bool load_from_file(const std::string& path);

    [[nodiscard]] const std::string& get_source_path() const { return m_source_path; }

    [[nodiscard]] bool has_found_error() const { return m_found_error; }

    void on_start() override;
    void on_stop() override;

    void step(float time_step) override;

private:
    bool call_lua_function(int nargs, int nresults);
    void set_lua_mouse_pos(float x, float y);

    const char* lua_get_joint_name(lua_State* state, int idx);
    static LuaRobotAI* lua_get_robot_ai(lua_State* state);
    static int lua_set_motor_speed(lua_State *state);
    static int lua_get_motor_speed(lua_State *state);
    static int lua_get_motor_min_speed(lua_State *state);
    static int lua_get_motor_max_speed(lua_State *state);

    static int lua_get_sensor_value(lua_State *state);

private:
    std::string m_source_path;
    lua_State *m_lua_state = nullptr;
    bool m_found_error = false;
};// class RobotAI
