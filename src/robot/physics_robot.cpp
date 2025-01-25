#include "physics_robot.hpp"
#include "parts/joint.hpp"
#include "parts/sensor.hpp"
#include "parts/shape.hpp"

#include <spdlog/spdlog.h>

PhysicsRobot::PhysicsRobot(const std::shared_ptr<Robot> &robot, b2World *world, b2Body *ground_body, bool lax)
    : PhysicsRobot(robot, {0.f, 0.f}, 0.f, world, ground_body, lax) {}

PhysicsRobot::PhysicsRobot(const std::shared_ptr<Robot> &robot, const glm::vec2 &position, float angle, b2World *world, b2Body *ground_body, bool lax)
    : m_robot(robot), m_world(world), m_ground_body(ground_body) {
    assert(m_robot != nullptr);
    assert(m_world != nullptr);

    BuildContext ctx{};
    ctx.position = position;
    ctx.angle = angle;
    ctx.lax = lax;
    build(ctx);

    auto *main_body_part = robot->get_part_by_name("body");
    if (main_body_part == nullptr && !robot->get_parts().empty())// no body found, get the first part
        main_body_part = robot->get_parts()[0].get();
    if (main_body_part != nullptr)
        m_main_body = m_bodies[main_body_part];
}

void PhysicsRobot::sync_to_box2d() {
    for (const auto &[part, body]: m_bodies) {
        const auto &position = part->get_position();
        const auto &angle = part->get_angle();

        body->SetTransform(b2Vec2(position.x, position.y), glm::radians(angle));
    }
}

void PhysicsRobot::sync_from_box2d() {
    for (const auto &[part, body]: m_bodies) {
        const auto &position = body->GetPosition();
        const auto &angle = body->GetAngle();

        // TODO: Take into account the position and angle of the whole robot.
        part->set_position({position.x, position.y});
        part->set_angle(glm::degrees(angle));
    }
}

glm::vec2 PhysicsRobot::get_position() const {
    auto [x, y] = m_main_body->GetWorldCenter();
    return {x, y};
}

b2Body *PhysicsRobot::get_physics_body(Part *part) const {
    assert(part != nullptr);
    const auto it = m_bodies.find(part);
    if (it == m_bodies.end())
        return nullptr;
    return it->second;
}

b2Joint *PhysicsRobot::get_physics_joint(Joint *part) const {
    assert(part != nullptr);
    const auto it = m_joints.find(part);
    if (it == m_joints.end())
        return nullptr;
    return it->second;
}

b2Body *PhysicsRobot::get_physics_body_by_name(const std::string &name) const {
    auto *part = m_robot->get_part_by_name(name);
    if (part == nullptr)
        return nullptr;
    return get_physics_body(part);
}

b2Joint *PhysicsRobot::get_physics_joint_by_name(const std::string &name) const {
    auto *part = m_robot->get_part_by_name<Joint>(name);
    if (part == nullptr)
        return nullptr;
    return get_physics_joint(part);
}

bool PhysicsRobot::has_collision() const {
    for (const auto &[part, body]: m_bodies) {
        for (b2ContactEdge *edge = body->GetContactList(); edge != nullptr; edge = edge->next) {
            if (edge->contact->IsTouching())
                return true;
        }
    }

    return false;
}

bool PhysicsRobot::has_collision(Part *part) const {
    b2Body *body = get_physics_body(part);
    if (body == nullptr)
        return false;

    for (b2ContactEdge *edge = body->GetContactList(); edge != nullptr; edge = edge->next) {
        if (edge->contact->IsTouching())
            return true;
    }

    return false;
}

bool PhysicsRobot::has_collision_by_name(const std::string &name) const {
    auto *part = m_robot->get_part_by_name(name);
    if (part == nullptr) {
        SPDLOG_WARN("Part '{}' not found.", name);
        return false;
    }

    return has_collision(part);
}

static float better_fmod(float x, float y) {
    x = std::fmod(x, y);
    if (x < 0.0f)
        x += y;
    return x;
}

static float normalize_radians(float x) {
    return better_fmod(glm::degrees(x), 360.f);
}

float PhysicsRobot::get_sensor_value(const std::string &name) const {
    auto *part = m_robot->get_part_by_name(name);
    if (part == nullptr) {
        SPDLOG_WARN("Sensor part '{}' not found.", name);
        return 0.f;
    }

    if (auto *sensor = dynamic_cast<Sensor *>(part); sensor != nullptr) {
        return get_sensor_value(sensor);
    } else if (auto *joint = dynamic_cast<Joint *>(part); joint != nullptr) {
        return get_sensor_value(joint);
    } else {
        SPDLOG_WARN("Unsupported part type (cannot be a sensor) for '{}'.", name);
        return 0.f;
    }
}

class DistanceSensorRayCastCallback : public b2RayCastCallback {
public:
    bool found = false;
    b2Vec2 recorded_hit_point = b2Vec2_zero;

    float ReportFixture(b2Fixture *fixture, const b2Vec2 &point,
                        const b2Vec2 &normal, float fraction) override {
        // Nearest fixture hit.
        found = true;
        recorded_hit_point = point;
        return fraction;
    }
};// DistanceSensorRayCastCallback

float PhysicsRobot::get_sensor_value(Sensor *sensor) const {
    auto *part = m_robot->get_part_by_name(sensor->get_part());
    if (part == nullptr) {
        SPDLOG_WARN("Sensor attached part '{}' not found for '{}'.", sensor->get_part(), sensor->get_name());
        return 0.f;
    }

    auto *part_body = get_physics_body(part);
    if (part_body == nullptr) {
        SPDLOG_WARN("Physics body not found for '{}' (probably not a shape).", part->get_name());
        return 0.f;
    }

    // Attach point in world coordinates.
    const auto world_point = part_body->GetWorldPoint(b2Vec2(sensor->get_local_anchor().x, sensor->get_local_anchor().y));

    float true_value;
    if (auto *angle_sensor = dynamic_cast<AngleSensor *>(sensor); angle_sensor != nullptr) {
        true_value = normalize_radians(part_body->GetAngle());
    } else if (auto *distance_sensor = dynamic_cast<DistanceSensor *>(sensor); distance_sensor != nullptr) {
        float max_distance = distance_sensor->get_max_distance();
        if (max_distance == INFINITY)
            max_distance = 100000.f;

        const float angle = part_body->GetAngle() + glm::radians(distance_sensor->get_angle());
        const auto direction = b2Vec2(std::cos(angle), std::sin(angle));
        const auto ray_start = world_point;
        const auto ray_end = world_point + max_distance * direction;

        // By default, Box2D ignore shapes that contain the starting point of the ray.
        DistanceSensorRayCastCallback callback;
        m_world->RayCast(&callback, ray_start, ray_end);

        // Detected distance (in meters).
        float distance;
        if (callback.found)
            distance = (callback.recorded_hit_point - ray_start).Length();
        else
            distance = INFINITY;// no detection

        // If the hit is too near, we consider it as no detection.
        if (distance < distance_sensor->get_min_distance())
            distance = INFINITY;// no detection

        true_value = distance;
    } else {
        SPDLOG_WARN("Unsupported sensor type (cannot be a sensor) for '{}'.", sensor->get_name());
        return 0.f;
    }

    return true_value;
}

float PhysicsRobot::get_sensor_value(Joint *joint) const {
    b2Joint *physics_joint = m_joints.at(joint);
    assert(physics_joint != nullptr);

    float true_value;
    if (physics_joint->GetType() == e_revoluteJoint) {
        true_value = normalize_radians(static_cast<b2RevoluteJoint *>(physics_joint)->GetJointAngle());
    } else if (physics_joint->GetType() == e_prismaticJoint) {
        true_value = static_cast<b2PrismaticJoint *>(physics_joint)->GetJointTranslation();
    } else {
        SPDLOG_WARN("Unsupported joint type (cannot be a sensor) for '{}'.", joint->get_name());
        return 0.f;
    }

    // Apply noise if requested to the true value.
    if (joint->has_accuracy_info() && m_noisy)
        return joint->get_accuracy_info().apply(true_value);
    else
        return true_value;
}

bool PhysicsRobot::is_motor_enabled(const std::string &name) const {
    b2Joint *joint = get_physics_joint_by_name(name);
    if (joint == nullptr) {
        SPDLOG_WARN("Joint part '{}' not found.", name);
        return false;
    }

    if (joint->GetType() == e_revoluteJoint) {
        return static_cast<b2RevoluteJoint *>(joint)->IsMotorEnabled();
    } else if (joint->GetType() == e_prismaticJoint) {
        return static_cast<b2PrismaticJoint *>(joint)->IsMotorEnabled();
    }

    SPDLOG_WARN("Unsupported joint type (cannot be a motor) for '{}'.", name);
    return false;
}

void PhysicsRobot::set_motor_enabled(const std::string &name, bool enabled) {
    b2Joint *joint = get_physics_joint_by_name(name);
    if (joint == nullptr) {
        SPDLOG_WARN("Joint part '{}' not found.", name);
        return;
    }

    if (joint->GetType() == e_revoluteJoint) {
        static_cast<b2RevoluteJoint *>(joint)->EnableMotor(enabled);
    } else if (joint->GetType() == e_prismaticJoint) {
        static_cast<b2PrismaticJoint *>(joint)->EnableMotor(enabled);
    } else {
        SPDLOG_WARN("Unsupported joint type (cannot be a motor) for '{}'.", name);
    }
}

float PhysicsRobot::get_motor_speed(const std::string &name) const {
    b2Joint *joint = get_physics_joint_by_name(name);
    if (joint == nullptr) {
        SPDLOG_WARN("Joint part '{}' not found.", name);
        return 0.f;
    }

    if (joint->GetType() == e_revoluteJoint) {
        return glm::degrees(static_cast<b2RevoluteJoint *>(joint)->GetMotorSpeed());
    } else if (joint->GetType() == e_prismaticJoint) {
        return static_cast<b2PrismaticJoint *>(joint)->GetMotorSpeed();
    }

    SPDLOG_WARN("Unsupported joint type (cannot be a motor) for '{}'.", name);
    return 0.f;
}

glm::vec2 PhysicsRobot::get_motor_speed_range(const std::string &name) const {
    auto *joint = m_robot->get_part_by_name<Joint>(name);
    if (joint == nullptr) {
        SPDLOG_WARN("Joint part '{}' not found.", name);
        return {0.f, 0.f};
    }

    // FIXME: avoid using dynamic_cast
    if (auto *revolute_joint = dynamic_cast<RevoluteJoint *>(joint); revolute_joint != nullptr) {
        return {revolute_joint->get_min_motor_speed(), revolute_joint->get_max_motor_speed()};
    } else if (auto *prismatic_joint = dynamic_cast<PrismaticJoint *>(joint); prismatic_joint != nullptr) {
        return {prismatic_joint->get_min_motor_speed(), prismatic_joint->get_max_motor_speed()};
    } else {
        SPDLOG_WARN("Unsupported joint type (cannot be a motor) for '{}'.", name);
        return {0.f, 0.f};
    }
}

float PhysicsRobot::get_motor_max_force(const std::string &name) const {
    b2Joint *joint = get_physics_joint_by_name(name);
    if (joint == nullptr) {
        SPDLOG_WARN("Joint part '{}' not found.", name);
        return 0.f;
    }

    if (joint->GetType() == e_revoluteJoint) {
        return static_cast<b2RevoluteJoint *>(joint)->GetMaxMotorTorque();
    } else if (joint->GetType() == e_prismaticJoint) {
        return static_cast<b2PrismaticJoint *>(joint)->GetMaxMotorForce();
    }

    SPDLOG_WARN("Unsupported joint type (cannot be a motor) for '{}'.", name);
    return 0.f;
}

void PhysicsRobot::set_motor_speed(const std::string &name, float value) {
    auto *joint = m_robot->get_part_by_name<Joint>(name);
    if (joint == nullptr) {
        SPDLOG_WARN("Joint part '{}' not found.", name);
        return;
    }

    b2Joint *physics_joint = m_joints[joint];
    assert(physics_joint != nullptr);

    // Clamp the motor speed to the allowed range.
    const auto &range = get_motor_speed_range(name);
    value = std::clamp(value, range.x, range.y);

    if (physics_joint->GetType() == e_revoluteJoint) {
        static_cast<b2RevoluteJoint *>(physics_joint)->SetMotorSpeed(glm::radians(value));
    } else if (physics_joint->GetType() == e_prismaticJoint) {
        static_cast<b2PrismaticJoint *>(physics_joint)->SetMotorSpeed(value);
    } else {
        SPDLOG_WARN("Unsupported joint type (cannot be a motor) for '{}'.", name);
    }
}

void PhysicsRobot::clear() {
    // Destroy all the Box2D bodies and joints.

    for ([[maybe_unused]] const auto &[part, body]: m_bodies) {
        m_world->DestroyBody(body);
    }

    for ([[maybe_unused]] const auto &[part, joint]: m_joints) {
        m_world->DestroyJoint(joint);
    }

    // Clear the mapping.
    m_bodies.clear();
    m_joints.clear();
}

void PhysicsRobot::build(const BuildContext &ctx) {
    clear();

    for (const auto &part: m_robot->get_parts()) {
        build_part(ctx, part.get());
    }
}

void PhysicsRobot::build_part(const BuildContext &ctx, Part *part) {
    assert(part != nullptr);

    if (auto *shape = dynamic_cast<Shape *>(part); shape != nullptr) {
        build_shape_part(ctx, shape);
    } else if (auto *joint = dynamic_cast<Joint *>(part); joint != nullptr) {
        build_joint_part(ctx, joint);
    } else if (auto *led = dynamic_cast<Led *>(part); led != nullptr) {
        // Nothing to do, the LED has no physics representation.
    } else if (auto *ground = dynamic_cast<Ground *>(part); ground != nullptr) {
        m_bodies[ground] = m_ground_body;
    } else if (auto *sensor = dynamic_cast<Sensor *>(part); sensor != nullptr) {
        // Nothing to do, the sensor has no physics representation.
    } else {
        assert(false && "Unknown part type");
    }
}

void PhysicsRobot::build_shape_part(const BuildContext &ctx, Shape *part) {
    assert(part != nullptr);

    if (m_bodies.contains(part)) {
        // The part is already built.
        return;
    }

    b2BodyDef body_def;
    body_def.type = b2_dynamicBody;
    body_def.position.Set(part->get_position().x + ctx.position.x, part->get_position().y + ctx.position.y);
    body_def.angle = glm::radians(part->get_angle() + ctx.angle);
    body_def.userData.pointer = reinterpret_cast<uintptr_t>(part);

    b2Body *body = m_world->CreateBody(&body_def);
    m_bodies[part] = body;

    b2FixtureDef fixture_def;
    fixture_def.density = part->get_density();
    fixture_def.friction = part->get_friction();
    fixture_def.restitution = part->get_restitution();
    fixture_def.restitutionThreshold = part->get_restitution_threshold();
    fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(part);

    if (auto *shape = dynamic_cast<CircleShape *>(part); shape != nullptr) {
        b2CircleShape physics_shape;
        physics_shape.m_p = b2Vec2_zero;
        physics_shape.m_radius = shape->get_radius();
        fixture_def.shape = &physics_shape;
        body->CreateFixture(&fixture_def);
    } else if (auto *shape = dynamic_cast<RectangleShape *>(part); shape != nullptr) {
        b2PolygonShape physics_shape;
        physics_shape.SetAsBox(shape->get_size().x / 2.0f, shape->get_size().y / 2.0f);
        physics_shape.m_radius = 0.0001f;// TODO: avoid changing the Box2D skin radius (for CCD).
        fixture_def.shape = &physics_shape;
        body->CreateFixture(&fixture_def);
    } else if (auto *shape = dynamic_cast<PolygonShape *>(part); shape != nullptr) {
        b2PolygonShape physics_shape;
        physics_shape.Set(reinterpret_cast<const b2Vec2 *>(shape->get_vertices().data()), (int) shape->get_vertices().size());
        physics_shape.m_radius = 0.0001f;// TODO: avoid changing the Box2D skin radius (for CCD).
        fixture_def.shape = &physics_shape;
        body->CreateFixture(&fixture_def);
    } else {
        assert(false && "Unknown shape type");
    }
}

void PhysicsRobot::build_joint_part(const BuildContext &ctx, Joint *part) {
    assert(part != nullptr);

    if (m_joints.contains(part)) {
        // The part is already built.
        return;
    }

    Part *part_a = m_robot->get_part_by_name(part->get_part_a());
    Part *part_b = m_robot->get_part_by_name(part->get_part_b());

    if (ctx.lax && (part_a == nullptr || part_b == nullptr))
        return;

    assert(part_a != nullptr && part_b != nullptr);

    build_part(ctx, part_a);
    build_part(ctx, part_b);

    b2Body *body_a = get_physics_body(part_a);
    b2Body *body_b = get_physics_body(part_b);

    if (ctx.lax && (body_a == nullptr || body_b == nullptr))
        return;

    assert(body_a != nullptr && body_b != nullptr);

    if (auto *joint = dynamic_cast<DistanceJoint *>(part); joint != nullptr) {
        b2DistanceJointDef joint_def;
        joint_def.bodyA = body_a;
        joint_def.bodyB = body_b;
        joint_def.localAnchorA.Set(part->get_local_anchor_a().x, part->get_local_anchor_a().y);
        joint_def.localAnchorB.Set(part->get_local_anchor_b().x, part->get_local_anchor_b().y);
        joint_def.collideConnected = false;
        joint_def.length = joint->get_length();
        joint_def.minLength = joint->get_min_length();
        joint_def.maxLength = joint->get_max_length();
        joint_def.collideConnected = joint->should_collide();
        m_joints[part] = m_world->CreateJoint(&joint_def);
    } else if (auto *joint = dynamic_cast<RevoluteJoint *>(part); joint != nullptr) {
        b2RevoluteJointDef joint_def;
        joint_def.bodyA = body_a;
        joint_def.bodyB = body_b;
        joint_def.localAnchorA.Set(joint->get_local_anchor_a().x, joint->get_local_anchor_a().y);
        joint_def.localAnchorB.Set(joint->get_local_anchor_b().x, joint->get_local_anchor_b().y);
        joint_def.collideConnected = false;
        joint_def.enableLimit = joint->is_limit_enabled();
        joint_def.lowerAngle = glm::radians(joint->get_lower_angle());
        joint_def.upperAngle = glm::radians(joint->get_upper_angle());
        joint_def.enableMotor = joint->is_motor();

        // If 0 is a valid motor speed, use it by default (motor turned off). Otherwise, use the minimum motor speed.
        if (joint->get_min_motor_speed() <= 0.f && joint->get_max_motor_speed() >= 0.f) {
            joint_def.motorSpeed = 0.f;
        } else {
            joint_def.motorSpeed = glm::radians(joint->get_min_motor_speed());
        }

        joint_def.maxMotorTorque = joint->get_max_motor_torque();
        joint_def.collideConnected = joint->should_collide();
        m_joints[part] = m_world->CreateJoint(&joint_def);
    } else if (auto *joint = dynamic_cast<PrismaticJoint *>(part); joint != nullptr) {
        b2PrismaticJointDef joint_def;
        joint_def.bodyA = body_a;
        joint_def.bodyB = body_b;
        joint_def.localAnchorA.Set(joint->get_local_anchor_a().x, joint->get_local_anchor_a().y);
        joint_def.localAnchorB.Set(joint->get_local_anchor_b().x, joint->get_local_anchor_b().y);
        joint_def.localAxisA.Set(joint->get_local_axis_a().x, joint->get_local_axis_a().y);
        joint_def.enableLimit = joint->is_limit_enabled();
        joint_def.lowerTranslation = joint->get_lower_translation();
        joint_def.upperTranslation = joint->get_upper_translation();

        joint_def.enableMotor = joint->is_motor();

        // If 0 is a valid motor speed, use it by default (motor turned off). Otherwise, use the minimum motor speed.
        if (joint->get_min_motor_speed() <= 0.f && joint->get_max_motor_speed() >= 0.f) {
            joint_def.motorSpeed = 0.f;
        } else {
            joint_def.motorSpeed = glm::radians(joint->get_min_motor_speed());
        }

        joint_def.maxMotorForce = joint->get_max_motor_force();
        joint_def.collideConnected = joint->should_collide();
        m_joints[part] = m_world->CreateJoint(&joint_def);
    } else {
        assert(false && "Unknown joint type");
    }
}
