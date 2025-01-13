#include "robot/robot_ai.hpp"

extern "C" {
#include "robot_c/robot.h"
}

#include <fmt/format.h>
#include <imgui.h>

class SpiderRobotAI : public RobotAI {
public:
    SpiderRobotAI() {
        Robot__robot_reset(&m_memory);
    }

    void on_start() override {
        Robot__robot_reset(&m_memory);
    }

#define INPUTS                                                                \
    X(float, hipLangle, physics_robot->get_sensor_value("hipL"))              \
    X(float, hipRangle, physics_robot->get_sensor_value("hipR"))              \
    X(float, kneeLangle, physics_robot->get_sensor_value("kneeL"))            \
    X(float, kneeRangle, physics_robot->get_sensor_value("kneeR"))            \
    X(float, arm1angle, physics_robot->get_sensor_value("shoulder"))          \
    X(float, arm2angle, physics_robot->get_sensor_value("elbow1"))            \
    X(float, arm3angle, physics_robot->get_sensor_value("elbow2"))            \
    X(float, arm4angle, physics_robot->get_sensor_value("elbow3"))            \
    X(float, gyroscope, physics_robot->get_sensor_value("gyroscope"))         \
    X(float, calfLdistance, physics_robot->get_sensor_value("calfLDistance")) \
    X(float, calfRdistance, physics_robot->get_sensor_value("calfRDistance")) \
    X(float, bodyDistance, physics_robot->get_sensor_value("bodyDistance")) \
    X(bool, footLcontact, physics_robot->has_collision_by_name("calfL"))      \
    X(bool, footRcontact, physics_robot->has_collision_by_name("calfR"))      \
    X(bool, bodyContact, physics_robot->has_collision_by_name("body"))

#define OUTPUTS              \
    X(hipLspeed, physics_robot->set_motor_speed("hipL", m_out.hipLspeed))     \
    X(hipRspeed, physics_robot->set_motor_speed("hipR", m_out.hipRspeed))     \
    X(kneeLspeed, physics_robot->set_motor_speed("kneeL", m_out.kneeLspeed))   \
    X(kneeRspeed, physics_robot->set_motor_speed("kneeR", m_out.kneeRspeed))   \
    X(arm1speed, physics_robot->set_motor_speed("shoulder", m_out.arm1speed)) \
    X(arm2speed, physics_robot->set_motor_speed("elbow1", m_out.arm2speed))   \
    X(arm3speed, physics_robot->set_motor_speed("elbow2", m_out.arm3speed))   \
    X(arm4speed, physics_robot->set_motor_speed("elbow3", m_out.arm4speed))   \
    X(arm_target_x, /* nothing */)   \
    X(arm_target_y, /* nothing */)   \

    void step(float dt) override {
        auto *physics_robot = get_physics_robot();

        // Input parameters
#define X(typ, name, value) const typ name = value;
        INPUTS
#undef X
        
        // Call lustre code
        Robot__robot_step(dt,
                          hipLangle, hipRangle, kneeLangle, kneeRangle,
                          arm1angle, arm2angle, arm3angle, arm4angle,
                          gyroscope, calfLdistance, calfRdistance, bodyDistance, footLcontact, footRcontact, bodyContact,
                          m_user_input.main_x, m_user_input.main_y, m_user_input.secondary_x, m_user_input.secondary_y, m_user_input.action_a,
                          &m_out, &m_memory);

        // Output parameters
#define X(varName, setCode) setCode;
        OUTPUTS
#undef X
    }

    void show_state() override {
        ImGui::Text("Spider robot AI (Lustre code)");

        auto *physics_robot = get_physics_robot();

        ImGui::SeparatorText("Input parameters");
#define X(typ, name, value) ImGui::Text(fmt::format("{}: {}", #name, value).c_str());
        INPUTS
#undef X

        ImGui::SeparatorText("Output parameters");
#define X(varName, setCode) ImGui::Text(fmt::format("{}: {}", #varName, m_out.varName).c_str());
        OUTPUTS
#undef X
    }

private:
    Robot__robot_mem m_memory;
    Robot__robot_out m_out;
};// class SpiderRobotAI

extern "C" RobotAI *create_robot_ai() {
    return new SpiderRobotAI();
}
