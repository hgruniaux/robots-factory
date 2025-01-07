#include "world_description.hpp"

#include "parser_utils.hpp"
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

bool load_shape_description(b2World *world, b2Body *body, const YAML::Node &node) {
    assert(world != nullptr && body != nullptr);

    if (!node["type"])
        throw YAML::RepresentationException(node.Mark(), "Shape type not defined");

    const std::string &type = node["type"].as<std::string>();
    if (type == "box") {
        b2PolygonShape shape;

        const YAML::Node &size_node = node["size"];
        if (size_node.IsScalar()) {
            const float half_extend = size_node.as<float>() / 2.f;
            shape.SetAsBox(half_extend, half_extend);
        } else {
            glm::vec2 size;
            from_yaml(size_node, size);
            shape.SetAsBox(size.x / 2.f, size.y / 2.f);
        }

        // TODO: avoid changing the Box2D skin radius (for CCD).
        shape.m_radius = 0.0001f;

        b2FixtureDef fixture_def;
        fixture_def.shape = &shape;

        if (node["density"])
            fixture_def.density = node["density"].as<float>();
        if (node["friction"])
            fixture_def.friction = node["friction"].as<float>();
        if (node["restitution"])
            fixture_def.restitution = node["restitution"].as<float>();
        if (node["restitution_threshold"])
            fixture_def.restitutionThreshold = node["restitution_threshold"].as<float>();

        body->CreateFixture(&fixture_def);
    } else {
        throw YAML::RepresentationException(node["type"].Mark(), "Invalid shape type '" + type + "'");
    }

    return true;
}

bool load_body_description(b2World *world, const YAML::Node &node) {
    assert(world != nullptr);

    if (!node["type"])
        throw YAML::RepresentationException(node.Mark(), "Body type not defined");

    const std::string &type = node["type"].as<std::string>();
    b2BodyDef body_def;
    if (type == "static") {
        body_def.type = b2_staticBody;
    } else if (type == "dynamic") {
        body_def.type = b2_dynamicBody;
    } else if (type == "kinematic") {
        body_def.type = b2_kinematicBody;
    } else {
        throw YAML::RepresentationException(node["type"].Mark(), "Invalid body type '" + type + "'");
    }

    if (node["position"]) {
        glm::vec2 position;
        from_yaml(node["position"], position);
        body_def.position.Set(position.x, position.y);
    }

    if (node["linear_velocity"]) {
        glm::vec2 linear_velocity;
        from_yaml(node["linear_velocity"], linear_velocity);
        body_def.linearVelocity.Set(linear_velocity.x, linear_velocity.y);
    }

    if (node["angle"])
        body_def.angle = glm::degrees(node["angle"].as<float>());
    if (node["linear_damping"])
        body_def.linearDamping = node["linear_damping"].as<float>();
    if (node["angular_damping"])
        body_def.angularDamping = node["angular_damping"].as<float>();
    if (node["gravity_scale"])
        body_def.gravityScale = node["gravity_scale"].as<float>();
    if (node["fixed_rotation"])
        body_def.fixedRotation = node["fixed_rotation"].as<bool>();
    if (node["bullet"])
        body_def.bullet = node["bullet"].as<bool>();

    b2Body *body = world->CreateBody(&body_def);
    if (body == nullptr) {
        SPDLOG_ERROR("Failed to create static body");
        return false;
    }

    if (node["shapes"]) {
        const YAML::Node &shapes = node["shapes"];
        if (!shapes.IsSequence()) throw YAML::RepresentationException(shapes.Mark(), "Invalid shapes in body description");


        for (const auto &shape: shapes) {
            if (!load_shape_description(world, body, shape))
                return false;
        }
    }

    return true;
}

bool load_world_description(b2World *world, const YAML::Node &root) {
    assert(world != nullptr);

    // Load gravity
    if (root["gravity"]) {
        const YAML::Node &gravity = root["gravity"];
        if (gravity.IsSequence() && gravity.size() == 2) {
            world->SetGravity(b2Vec2(gravity[0].as<float>(), gravity[1].as<float>()));
        } else if (gravity.IsScalar()) {
            world->SetGravity(b2Vec2(0.0f, gravity.as<float>()));
        } else {
            throw YAML::RepresentationException(gravity.Mark(), "Invalid gravity in world description");
        }
    }

    // Load bodies
    if (root["bodies"]) {
        const YAML::Node &bodies = root["bodies"];
        if (!bodies.IsSequence()) {
            throw YAML::RepresentationException(bodies.Mark(), "Invalid bodies in world description");
        }

        for (const auto &body: bodies) {
            if (!load_body_description(world, body))
                return false;
        }
    }

    return true;
}

bool load_world_description(b2World *world, const std::string &path) {
    assert(world != nullptr);

    try {
        YAML::Node root = YAML::LoadFile(path);
        if (!root.IsDefined()) {
            SPDLOG_ERROR("Failed to load world description from '{}'", path);
            return false;
        }

        return load_world_description(world, root);
    } catch (const YAML::Exception &err) {
        if (err.mark.is_null())
            SPDLOG_ERROR("Failed to load world description\n{}: {}", path, err.what());
        else
            SPDLOG_ERROR("Failed to load world description\n{}:{}:{}: {}", path, err.mark.line, err.mark.column, err.what());
        return false;
    }
}
