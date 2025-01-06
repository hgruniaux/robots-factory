#include "robot_ai.hpp"
#include "parts/joint.hpp"
#include "parts/sensor.hpp"

void RobotAI::attach(const std::shared_ptr<Robot> &robot, PhysicsRobot *physics_robot) {
    assert(robot != nullptr);
    assert(physics_robot != nullptr);

    m_robot = robot;
    m_physics_robot = physics_robot;
}

void RobotAI::dettach() {
    m_robot = nullptr;
    m_physics_robot = nullptr;
}

float RobotAI::get_sensor_value(const std::string &name) const {
    auto *sensor = m_robot->get_part_by_name(name);
    assert(sensor != nullptr && "Part not found.");

    if (auto *revolute_joint = dynamic_cast<RevoluteJoint *>(sensor); revolute_joint != nullptr) {
        auto *physics_joint = get_physics_joint<b2RevoluteJoint>(revolute_joint);
        return glm::degrees(physics_joint->GetJointAngle());
    } else if (auto *prismatic_joint = dynamic_cast<PrismaticJoint *>(sensor); prismatic_joint != nullptr) {
        auto *physics_joint = get_physics_joint<b2PrismaticJoint>(prismatic_joint);
        return physics_joint->GetJointTranslation();
    } else {
        assert(false && "Unsupported sensor type.");
    }

    return 0.0f;
}

float RobotAI::get_motor_speed(const std::string &name) const {
    auto *joint = m_robot->get_part_by_name<Joint>(name);
    assert(joint != nullptr && "Joint not found or is not a joint.");

    if (auto *revolute_joint = dynamic_cast<RevoluteJoint *>(joint); revolute_joint != nullptr) {
        assert(revolute_joint->is_motor() && "Joint is not a motor.");

        auto *physics_joint = get_physics_joint<b2RevoluteJoint>(revolute_joint);
        return glm::degrees(physics_joint->GetMotorSpeed());
    } else if (auto *prismatic_joint = dynamic_cast<PrismaticJoint *>(joint); prismatic_joint != nullptr) {
        assert(prismatic_joint->is_motor() && "Joint is not a motor.");
        auto *physics_joint = get_physics_joint<b2PrismaticJoint>(prismatic_joint);
        return physics_joint->GetMotorSpeed();
    } else {
        assert(false && "Unsupported joint type (cannot be a motor).");
    }
}

float RobotAI::get_motor_min_speed(const std::string &name) const {
    auto *joint = m_robot->get_part_by_name<Joint>(name);
    assert(joint != nullptr && "Joint not found or is not a joint.");

    if (auto *revolute_joint = dynamic_cast<RevoluteJoint *>(joint); revolute_joint != nullptr) {
        assert(revolute_joint->is_motor() && "Joint is not a motor.");
        return revolute_joint->get_min_motor_speed();
    } else if (auto *prismatic_joint = dynamic_cast<PrismaticJoint *>(joint); prismatic_joint != nullptr) {
        assert(prismatic_joint->is_motor() && "Joint is not a motor.");
        return prismatic_joint->get_min_motor_speed();
    } else {
        assert(false && "Unsupported joint type (cannot be a motor).");
    }
}

float RobotAI::get_motor_max_speed(const std::string &name) const {
    auto *joint = m_robot->get_part_by_name<Joint>(name);
    assert(joint != nullptr && "Joint not found or is not a joint.");

    if (auto *revolute_joint = dynamic_cast<RevoluteJoint *>(joint); revolute_joint != nullptr) {
        assert(revolute_joint->is_motor() && "Joint is not a motor.");
        return revolute_joint->get_max_motor_speed();
    } else if (auto *prismatic_joint = dynamic_cast<PrismaticJoint *>(joint); prismatic_joint != nullptr) {
        assert(prismatic_joint->is_motor() && "Joint is not a motor.");
        return prismatic_joint->get_max_motor_speed();
    } else {
        assert(false && "Unsupported joint type (cannot be a motor).");
    }
}

void RobotAI::set_motor_speed(const std::string &name, float value) {
    auto *joint = m_robot->get_part_by_name<Joint>(name);
    assert(joint != nullptr && "Joint not found or is not a joint.");

    if (auto *revolute_joint = dynamic_cast<RevoluteJoint *>(joint); revolute_joint != nullptr) {
        assert(revolute_joint->is_motor() && "Joint is not a motor.");

        const float min_speed = revolute_joint->get_min_motor_speed();
        const float max_speed = revolute_joint->get_max_motor_speed();
        const float speed = glm::clamp(value, min_speed, max_speed);

        auto *physics_joint = get_physics_joint<b2RevoluteJoint>(revolute_joint);
        physics_joint->SetMotorSpeed(glm::radians(speed));
    } else if (auto *prismatic_joint = dynamic_cast<PrismaticJoint *>(joint); prismatic_joint != nullptr) {
        assert(prismatic_joint->is_motor() && "Joint is not a motor.");

        const float min_speed = prismatic_joint->get_min_motor_speed();
        const float max_speed = prismatic_joint->get_max_motor_speed();
        const float speed = glm::clamp(value, min_speed, max_speed);

        auto *physics_joint = get_physics_joint<b2PrismaticJoint>(prismatic_joint);
        physics_joint->SetMotorSpeed(speed);
    } else {
        assert(false && "Unsupported joint type (cannot be a motor).");
    }
}

b2Body *RobotAI::get_physics_body_by_name(const std::string &name) const {
    auto *part = m_robot->get_part_by_name(name);
    assert(part != nullptr && "Part not found.");
    return get_physics_body(part);
}

b2Joint *RobotAI::get_physics_joint_by_name(const std::string &name) const {
    auto *part = m_robot->get_part_by_name<Joint>(name);
    assert(part != nullptr && "Joint not found or is not a joint.");
    return get_physics_joint(part);
}
