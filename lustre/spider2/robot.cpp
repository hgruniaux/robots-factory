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
    X(bool, footLcontact, physics_robot->has_collision_by_name("calfL"))      \
    X(bool, footRcontact, physics_robot->has_collision_by_name("calfR"))      \
    X(bool, bodyContact, physics_robot->has_collision_by_name("body"))

#define OUTPUTS              \
    X(hipLspeed, "hipL")     \
    X(hipRspeed, "hipR")     \
    X(kneeLspeed, "kneeL")   \
    X(kneeRspeed, "kneeR")   \
    X(arm1speed, "shoulder") \
    X(arm2speed, "elbow1")   \
    X(arm3speed, "elbow2")   \
    X(arm4speed, "elbow3")

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
                          gyroscope, calfLdistance, calfRdistance, footLcontact, footRcontact, bodyContact,
                          m_user_input.main_x, m_user_input.main_y, m_user_input.secondary_x, m_user_input.secondary_y, m_user_input.action_a,
                          &m_out, &m_memory);

        // Output parameters
#define X(varName, motorName) physics_robot->set_motor_speed(motorName, m_out.varName);
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
#define X(varName, motorName) ImGui::Text(fmt::format("{}: {}", #varName, m_out.varName).c_str());
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
