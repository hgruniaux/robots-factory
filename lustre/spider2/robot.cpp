#include "robot/robot_ai.hpp"

extern "C" {
#include "robot_c/robot.h"
}

#include <fmt/format.h>

class SpiderRobotAI : public RobotAI {
public:
    SpiderRobotAI() {
        Robot__robot_reset(&m_memory);
    }

    void on_start() override {
        Robot__robot_reset(&m_memory);
    }

    void step(float dt) override {
        auto *physics_robot = get_physics_robot();

        // Input parameters
        const float hipLangle = physics_robot->get_sensor_value("hipL");
        const float hipRangle = physics_robot->get_sensor_value("hipR");
        const float kneeLangle = physics_robot->get_sensor_value("kneeL");
        const float kneeRangle = physics_robot->get_sensor_value("kneeR");
        const float gyroscope = physics_robot->get_sensor_value("gyroscope");
        const float calfLdistance = physics_robot->get_sensor_value("calfLDistance");
        const float calfRdistance = physics_robot->get_sensor_value("calfRDistance");
        const bool footLcontact = physics_robot->has_collision_by_name("calfL");
        const bool footRcontact = physics_robot->has_collision_by_name("calfR");
        const bool bodyContact = physics_robot->has_collision_by_name("body");

        const float arm1angle = physics_robot->get_sensor_value("shoulder");
        const float arm2angle = physics_robot->get_sensor_value("elbow1");
        const float arm3angle = physics_robot->get_sensor_value("elbow2");
        const float arm4angle = physics_robot->get_sensor_value("elbow3");

        // Call lustre code
        Robot__robot_out out;
        Robot__robot_step(dt,
                          hipLangle, hipRangle, kneeLangle, kneeRangle,
                          arm1angle, arm2angle, arm3angle, arm4angle,
                          gyroscope, calfLdistance, calfRdistance, footLcontact, footRcontact, bodyContact, &out, &m_memory);

        // Output parameters
        physics_robot->set_motor_speed("hipL", out.hipLspeed);
        physics_robot->set_motor_speed("hipR", out.hipRspeed);
        physics_robot->set_motor_speed("kneeL", out.kneeLspeed);
        physics_robot->set_motor_speed("kneeR", out.kneeRspeed);
        physics_robot->set_motor_speed("shoulder", out.arm1speed);
        physics_robot->set_motor_speed("elbow1", out.arm2speed);
        physics_robot->set_motor_speed("elbow2", out.arm3speed);
        physics_robot->set_motor_speed("elbow3", out.arm4speed);
    }

private:
    Robot__robot_mem m_memory;
};// class SpiderRobotAI

extern "C" RobotAI *create_robot_ai() {
    return new SpiderRobotAI();
}
