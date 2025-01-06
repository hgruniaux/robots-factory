#include "lua_robot_ai.hpp"

#include <spdlog/spdlog.h>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

LuaRobotAI::LuaRobotAI() : RobotAI() {
    m_lua_state = luaL_newstate();
    assert(m_lua_state != nullptr);

    // Load the Lua standard libraries.
    luaL_openlibs(m_lua_state);

    // Save a reference to this instance in the Lua state.
    lua_pushlightuserdata(m_lua_state, this);
    lua_setglobal(m_lua_state, "__robot_ai");

    set_lua_mouse_pos(0.0f, 0.0f);

    // Register the C functions that the Lua script can call.
    lua_register(m_lua_state, "set_motor_speed", &lua_set_motor_speed);
    lua_register(m_lua_state, "get_motor_speed", &lua_get_motor_speed);
    lua_register(m_lua_state, "get_motor_min_speed", &lua_get_motor_min_speed);
    lua_register(m_lua_state, "get_motor_max_speed", &lua_get_motor_max_speed);

    lua_register(m_lua_state, "get_sensor_value", &lua_get_sensor_value);
}

LuaRobotAI::~LuaRobotAI() {
    lua_close(m_lua_state);
}

bool LuaRobotAI::load_from_string(std::string_view code) {
    if (luaL_dostring(m_lua_state, code.data()) != LUA_OK) {
        const char *error = lua_tostring(m_lua_state, -1);
        lua_pop(m_lua_state, 1);
        SPDLOG_ERROR("Failed to load Lua script:\n{}", error);
        return false;
    }

    return true;
}

bool LuaRobotAI::load_from_file(const std::string &path) {
    m_source_path = path;

    if (luaL_dofile(m_lua_state, path.c_str()) != LUA_OK) {
        const char *error = lua_tostring(m_lua_state, -1);
        lua_pop(m_lua_state, 1);
        SPDLOG_ERROR("Failed to load Lua script:\n{}", error);
        return false;
    }

    return true;
}

void LuaRobotAI::on_start() {
    lua_getglobal(m_lua_state, "on_start");
    if (lua_isfunction(m_lua_state, -1)) {
        call_lua_function(0, 0);
    } else {
        lua_pop(m_lua_state, 1);
    }
}

void LuaRobotAI::on_stop() {
    lua_getglobal(m_lua_state, "on_stop");
    if (lua_isfunction(m_lua_state, -1)) {
        call_lua_function(0, 0);
    } else {
        lua_pop(m_lua_state, 1);
    }
}

void LuaRobotAI::step(float time_step) {
    lua_pushnumber(m_lua_state, time_step);
    lua_setglobal(m_lua_state, "delta_time");

    lua_getglobal(m_lua_state, "on_update");
    if (lua_isfunction(m_lua_state, -1)) {
        lua_pushnumber(m_lua_state, time_step);
        call_lua_function(1, 0);
    } else {
        lua_pop(m_lua_state, 1);
    }
}

bool LuaRobotAI::call_lua_function(int nargs, int nresults) {
    if (lua_pcall(m_lua_state, nargs, nresults, 0) == LUA_OK)
        return true;

    const char *error = lua_tostring(m_lua_state, -1);
    lua_pop(m_lua_state, 1);
    SPDLOG_ERROR("Failed to call Lua function:\n{}", error);
    m_found_error = true;
    return false;
}

void LuaRobotAI::set_lua_mouse_pos(float x, float y) {
    lua_pushnumber(m_lua_state, x);
    lua_setglobal(m_lua_state, "mouse_x");

    lua_pushnumber(m_lua_state, y);
    lua_setglobal(m_lua_state, "mouse_y");
}

LuaRobotAI *LuaRobotAI::lua_get_robot_ai(lua_State *state) {
    lua_getglobal(state, "__robot_ai");
    auto *robot_ai = (LuaRobotAI *) lua_touserdata(state, -1);
    lua_pop(state, 1);
    assert(robot_ai != nullptr);
    return robot_ai;
}

const char *LuaRobotAI::lua_get_joint_name(lua_State *state, int idx) {
    const char *name = lua_tostring(state, 1);
    if (name == nullptr) {
        lua_pushstring(state, "Invalid joint name.");
        lua_error(state);
        m_found_error = true;
        return nullptr;
    }

    return name;
}

int LuaRobotAI::lua_set_motor_speed(lua_State *state) {
    assert(state != nullptr);

    auto *robot_ai = lua_get_robot_ai(state);

    // Check the number of arguments.
    if (lua_gettop(state) != 2) {
        lua_pushstring(state, "Invalid number of arguments.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Check the type of the arguments.
    if (!lua_isstring(state, 1) || !lua_isnumber(state, 2)) {
        lua_pushstring(state, "Invalid argument types.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Get the name of the actuator.
    const char *name = robot_ai->lua_get_joint_name(state, 1);
    if (name == nullptr)
        return 0;

    // Get the value of the actuator.
    const auto value = (float) lua_tonumber(state, 2);

    // Transmit the call to the C++ world!
    robot_ai->set_motor_speed(name, value);
    return 0;
}

int LuaRobotAI::lua_get_motor_speed(lua_State *state) {
    assert(state != nullptr);

    auto *robot_ai = lua_get_robot_ai(state);

    // Check the number of arguments.
    if (lua_gettop(state) != 1) {
        lua_pushstring(state, "Invalid number of arguments.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Check the type of the arguments.
    if (!lua_isstring(state, 1)) {
        lua_pushstring(state, "Invalid argument types.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Get the name of the actuator.
    const char *name = robot_ai->lua_get_joint_name(state, 1);
    if (name == nullptr)
        return 0;

    // Transmit the call to the C++ world!
    float speed = robot_ai->get_motor_speed(name);
    lua_pushnumber(state, speed);
    return 1;
}

int LuaRobotAI::lua_get_motor_min_speed(lua_State *state) {
    assert(state != nullptr);

    auto *robot_ai = lua_get_robot_ai(state);

    // Check the number of arguments.
    if (lua_gettop(state) != 1) {
        lua_pushstring(state, "Invalid number of arguments.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Check the type of the arguments.
    if (!lua_isstring(state, 1)) {
        lua_pushstring(state, "Invalid argument types.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Get the name of the actuator.
    const char *name = robot_ai->lua_get_joint_name(state, 1);
    if (name == nullptr)
        return 0;

    // Transmit the call to the C++ world!
    float speed = robot_ai->get_motor_min_speed(name);
    lua_pushnumber(state, speed);
    return 1;
}

int LuaRobotAI::lua_get_motor_max_speed(lua_State *state) {
    assert(state != nullptr);

    auto *robot_ai = lua_get_robot_ai(state);

    // Check the number of arguments.
    if (lua_gettop(state) != 1) {
        lua_pushstring(state, "Invalid number of arguments.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Check the type of the arguments.
    if (!lua_isstring(state, 1)) {
        lua_pushstring(state, "Invalid argument types.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Get the name of the actuator.
    const char *name = robot_ai->lua_get_joint_name(state, 1);
    if (name == nullptr)
        return 0;

    // Transmit the call to the C++ world!
    float speed = robot_ai->get_motor_max_speed(name);
    lua_pushnumber(state, speed);
    return 1;
}

int LuaRobotAI::lua_get_sensor_value(lua_State *state) {
    assert(state != nullptr);

    auto *robot_ai = lua_get_robot_ai(state);

    // Check the number of arguments.
    if (lua_gettop(state) != 1) {
        lua_pushstring(state, "Invalid number of arguments.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Check the type of the arguments.
    if (!lua_isstring(state, 1)) {
        lua_pushstring(state, "Invalid argument types.");
        lua_error(state);
        robot_ai->m_found_error = true;
        return 0;
    }

    // Get the name of the sensor.
    const char *name = robot_ai->lua_get_joint_name(state, 1);
    if (name == nullptr)
        return 0;

    // Transmit the call to the C++ world!
    const auto value = robot_ai->get_sensor_value(name);
    lua_pushnumber(state, value);
    return 1;
}
