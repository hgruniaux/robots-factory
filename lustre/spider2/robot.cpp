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

        // Call lustre code
        Robot__robot_out out;
        Robot__robot_step(dt, hipLangle, hipRangle, kneeLangle, kneeRangle, gyroscope, calfLdistance, calfRdistance, &out, &m_memory);

        fmt::print("@@@@ STEP\n");
        fmt::print("hipLangle: {}\n", hipLangle);
        fmt::print("hipRangle: {}\n", hipRangle);
        fmt::print("kneeLangle: {}\n", kneeLangle);
        fmt::print("kneeRangle: {}\n", kneeRangle);

        // Output parameters
        physics_robot->set_motor_speed("hipL", out.hipLspeed);
        physics_robot->set_motor_speed("hipR", out.hipRspeed);
        physics_robot->set_motor_speed("kneeL", out.kneeLspeed);
        physics_robot->set_motor_speed("kneeR", out.kneeRspeed);
    }

private:
    Robot__robot_mem m_memory;
};// class SpiderRobotAI

extern "C" RobotAI* create_robot_ai() {
    return new SpiderRobotAI();
}
