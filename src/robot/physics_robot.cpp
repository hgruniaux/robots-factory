#include "physics_robot.hpp"
#include "parts/joint.hpp"
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
        fixture_def.shape = &physics_shape;
        body->CreateFixture(&fixture_def);
    } else if (auto *shape = dynamic_cast<PolygonShape *>(part); shape != nullptr) {
        b2PolygonShape physics_shape;
        physics_shape.Set(reinterpret_cast<const b2Vec2 *>(shape->get_vertices().data()), (int) shape->get_vertices().size());
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
