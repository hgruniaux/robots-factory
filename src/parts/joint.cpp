#include "joint.hpp"
#include "draw/renderer_2d.hpp"
#include "parser_utils.hpp"
#include "robot/robot.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

/**
 * A joint part.
 */

bool Joint::show_inspector() {
    bool changed = Part::show_inspector();

    if (ImGui::CollapsingHeader("Joint")) {
        changed |= ImGui::InputText("Part A", &m_part_a);
        ImGui::SetItemTooltip("The name of the first part to connect.");

        changed |= ImGui::InputText("Part B", &m_part_b);
        ImGui::SetItemTooltip("The name of the second part to connect.");

        if (ImGui::Button(ICON_FA_ROTATE " Swap parts")) {
            std::swap(m_part_a, m_part_b);
            changed = true;
        }

        const bool part_a_exists = get_robot()->get_part_by_name(m_part_a) != nullptr;
        if (!part_a_exists)
            ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, ICON_FA_TRIANGLE_EXCLAMATION " Part A does not exist!");

        const bool part_b_exists = get_robot()->get_part_by_name(m_part_b) != nullptr;
        if (!part_b_exists)
            ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, ICON_FA_TRIANGLE_EXCLAMATION " Part B does not exist!");

        if (m_part_a == m_part_b)
            ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, ICON_FA_TRIANGLE_EXCLAMATION " Part A and B can not be the same!");

        changed |= ImGui::DragFloat2("Local anchor A", &m_local_anchor_a[0], 0.01f);
        ImGui::SetItemTooltip("The local anchor point relative to part A center in meters.");
        changed |= ImGui::DragFloat2("Local anchor B", &m_local_anchor_b[0], 0.01f);
        ImGui::SetItemTooltip("The local anchor point relative to part B center in meters.");

        changed |= ImGui::Checkbox("Should collide", &m_should_collide);
        ImGui::SetItemTooltip("Whether the connected parts should collide with each other.");

        changed |= ImGui::Checkbox(ICON_FA_INFO " Has accuracy info", &m_has_accuracy_info);
        ImGui::SetItemTooltip("Whether the joint has accuracy information.");
    }

    if (m_has_accuracy_info)
        changed |= m_accuracy_info.show_inspector();

    return changed;
}

bool Joint::should_draw(const DrawPartContext &context) const {
    if (context.selected_part == this)
        return true;

    if (!context.draw_joints)
        return false;

    auto *part_a = get_robot()->get_part_by_name(m_part_a);
    auto *part_b = get_robot()->get_part_by_name(m_part_b);
    if (context.selected_part != part_a && context.selected_part != part_b && context.draw_only_selected_joints)
        return false;

    return true;
}

glm::vec2 Joint::get_world_anchor_a() const {
    auto *part_a = get_robot()->get_part_by_name(m_part_a);
    assert(part_a != nullptr);

    const float angle_a = glm::radians(part_a->get_angle());
    const float sin_a = std::sin(angle_a), cos_a = std::cos(angle_a);
    glm::vec2 world_anchor_a = part_a->get_position();
    world_anchor_a += glm::vec2(cos_a * m_local_anchor_a.x - sin_a * m_local_anchor_a.y,
                                sin_a * m_local_anchor_a.x + cos_a * m_local_anchor_a.y);
    return world_anchor_a;
}

glm::vec2 Joint::get_world_anchor_b() const {
    auto *part_b = get_robot()->get_part_by_name(m_part_b);
    assert(part_b != nullptr);

    const float angle_b = glm::radians(part_b->get_angle());
    const float sin_b = std::sin(angle_b), cos_b = std::cos(angle_b);
    glm::vec2 world_anchor_b = part_b->get_position();
    world_anchor_b += glm::vec2(cos_b * m_local_anchor_b.x - sin_b * m_local_anchor_b.y,
                                sin_b * m_local_anchor_b.x + cos_b * m_local_anchor_b.y);
    return world_anchor_b;
}

void Joint::draw(Renderer2D &renderer, const DrawPartContext &context) {
    if (!should_draw(context))
        return;

    Part::draw(renderer, context);

    auto *part_a = get_robot()->get_part_by_name(m_part_a);
    if (part_a != nullptr)
        renderer.draw_anchor(get_world_anchor_a());

    auto *part_b = get_robot()->get_part_by_name(m_part_b);
    if (part_b != nullptr)
        renderer.draw_anchor(get_world_anchor_b());
}

void Joint::load(const YAML::Node &node) {
    Part::load(node);

    if (node["part_a"])
        m_part_a = node["part_a"].as<std::string>();
    if (node["part_b"])
        m_part_b = node["part_b"].as<std::string>();

    if (node["local_anchor_a"])
        from_yaml(node["local_anchor_a"], m_local_anchor_a);
    if (node["local_anchor_b"])
        from_yaml(node["local_anchor_b"], m_local_anchor_b);

    if (node["should_collide"])
        m_should_collide = node["should_collide"].as<bool>();

    if (node["accuracy_info"]) {
        m_has_accuracy_info = true;
        m_accuracy_info.load(node["accuracy_info"]);
    }
}

void Joint::save(YAML::Emitter &emitter) const {
    Part::save(emitter);

    emitter << YAML::Key << "part_a" << YAML::Value << m_part_a;
    emitter << YAML::Key << "part_b" << YAML::Value << m_part_b;

    emitter << YAML::Key << "local_anchor_a" << YAML::Value;
    to_yaml(emitter, m_local_anchor_a);
    emitter << YAML::Key << "local_anchor_b" << YAML::Value;
    to_yaml(emitter, m_local_anchor_b);

    emitter << YAML::Key << "should_collide" << YAML::Value << m_should_collide;

    if (m_has_accuracy_info) {
        emitter << YAML::Key << "accuracy_info" << YAML::Value << YAML::BeginMap;
        m_accuracy_info.save(emitter);
        emitter << YAML::EndMap;
    }
}

/*
 * The DistanceJoint class
 */


bool DistanceJoint::show_inspector() {
    bool changed = Joint::show_inspector();

    if (ImGui::CollapsingHeader("Distance joint")) {
        changed |= ImGui::DragFloat("Length (in m)", &m_length, 0.01f, m_min_length, m_max_length);
        ImGui::SetItemTooltip("The desired length of the joint at rest in meters.");
        changed |= ImGui::DragFloat("Min length (in m)", &m_min_length, 0.01f, 0.f, m_max_length);
        ImGui::SetItemTooltip("The minimum length of the joint in meters.");
        changed |= ImGui::DragFloat("Max length (in m)", &m_max_length, 0.01f, m_min_length, INFINITY);
        ImGui::SetItemTooltip("The maximum length of the joint in meters.");
        changed |= ImGui::DragFloat("Frequency (in Hz)", &m_frequency, 1.f, 0.f, 30.f);
        ImGui::SetItemTooltip("The frequency of the spring in Hz.");
        changed |= ImGui::DragFloat("Damping ratio", &m_damping_ratio, 0.01f, 0.0f, 1.0f);
        ImGui::SetItemTooltip("The damping ratio of the spring.");

        ensure_constraints();
    }

    return changed;
}

void DistanceJoint::draw(Renderer2D &renderer, const DrawPartContext &context) {

    if (!should_draw(context))
        return;

    Joint::draw(renderer, context);

    auto *part_a = get_robot()->get_part_by_name(get_part_a());
    auto *part_b = get_robot()->get_part_by_name(get_part_b());
    if (part_a == nullptr || part_b == nullptr)
        return;

    const glm::vec2 world_anchor_a = get_world_anchor_a();
    const glm::vec2 world_anchor_b = get_world_anchor_b();
    const auto direction = glm::normalize(world_anchor_b - world_anchor_a);
    const auto normal = glm::vec2(-direction.y, direction.x);

    // Draw min and max markers
    constexpr float marker_size = 0.02f;
    constexpr float marker_width = 0.01f;
    constexpr glm::vec4 marker_color = {1.0f, 1.0f, 0.0f, 1.0f};
    const glm::vec2 marker_start = world_anchor_a - normal * marker_size;
    const glm::vec2 marker_end = world_anchor_a + normal * marker_size;
    renderer.draw_line(marker_start + direction * m_min_length, marker_end + direction * m_min_length, marker_width, marker_color);
    renderer.draw_line(marker_start + direction * m_max_length, marker_end + direction * m_max_length, marker_width, marker_color);

    // Draw the spring
    renderer.draw_line(world_anchor_a, world_anchor_a + direction * m_length, marker_width, marker_color);
}

void DistanceJoint::load(const YAML::Node &node) {
    Joint::load(node);

    if (node["length"])
        m_length = node["length"].as<float>();

    if (node["min_length"])
        m_min_length = node["min_length"].as<float>();
    else
        m_min_length = m_length;

    if (node["max_length"])
        m_max_length = node["max_length"].as<float>();
    else
        m_max_length = m_length;

    if (node["frequency"])
        m_frequency = node["frequency"].as<float>();
    if (node["damping_ratio"])
        m_damping_ratio = node["damping_ratio"].as<float>();

    ensure_constraints();
}

void DistanceJoint::save(YAML::Emitter &emitter) const {
    Joint::save(emitter);

    emitter << YAML::Key << "length" << YAML::Value << m_length;
    emitter << YAML::Key << "min_length" << YAML::Value << m_min_length;
    emitter << YAML::Key << "max_length" << YAML::Value << m_max_length;
    emitter << YAML::Key << "frequency" << YAML::Value << m_frequency;
    emitter << YAML::Key << "damping_ratio" << YAML::Value << m_damping_ratio;
}

void DistanceJoint::ensure_constraints() {
    m_length = std::clamp(m_length, m_min_length, m_max_length);
    m_min_length = std::min(m_min_length, m_max_length);
    m_max_length = std::max(m_min_length, m_max_length);
    m_frequency = std::clamp(m_frequency, 0.f, 30.f);
    m_damping_ratio = std::clamp(m_damping_ratio, 0.f, 1.f);
}

/*
 * The RevoluteJoint class
 */

bool RevoluteJoint::show_inspector() {
    bool changed = Joint::show_inspector();

    if (ImGui::CollapsingHeader("Revolute joint")) {
        changed |= ImGui::Checkbox("Limit enabled", &m_limit_enabled);
        ImGui::SetItemTooltip("Whether the joint has a rotation limit.");
        if (m_limit_enabled) {
            changed |= ImGui::DragFloat("Lower angle (in deg)", &m_lower_angle, 1.0f, -180.f, m_upper_angle);
            ImGui::SetItemTooltip("The lower angle limit in degrees.");
            changed |= ImGui::DragFloat("Upper angle (in deg)", &m_upper_angle, 1.0f, m_lower_angle, 180.f);
            ImGui::SetItemTooltip("The upper angle limit in degrees.");
        }

        changed |= ImGui::Checkbox("Is motor", &m_is_motor);
        ImGui::SetItemTooltip("Whether the joint is a motor.");
        if (m_is_motor) {
            changed |= ImGui::DragFloat("Min motor speed (in deg/s)", &m_min_motor_speed, 1.0f, 0.f, m_max_motor_speed);
            ImGui::SetItemTooltip("The minimum motor speed in degrees per second.");
            changed |= ImGui::DragFloat("Max motor speed (in deg/s)", &m_max_motor_speed, 1.0f, m_min_motor_speed, INFINITY);
            ImGui::SetItemTooltip("The maximum motor speed in degrees per second.");
            changed |= ImGui::DragFloat("Max motor torque", &m_max_motor_torque, 0.1f, 0.f, INFINITY);
            ImGui::SetItemTooltip("The maximum motor torque in N*m.");
        }

        ensure_constraints();
    }

    return changed;
}

void RevoluteJoint::draw(Renderer2D &renderer, const DrawPartContext &context) {
    if (!should_draw(context))
        return;

    Joint::draw(renderer, context);

    auto *part_a = get_robot()->get_part_by_name(get_part_a());
    if (m_limit_enabled && part_a != nullptr) {
        const float lower_angle = glm::radians(part_a->get_angle() + m_lower_angle);
        const float upper_angle = glm::radians(part_a->get_angle() + m_upper_angle);

        const glm::vec2 world_anchor_a = get_world_anchor_a();

        constexpr float marker_size = 0.04f;
        constexpr float marker_width = 0.01f;
        constexpr glm::vec3 marker_color = {1.0f, 1.0f, 0.0f};
        const glm::vec2 lower = world_anchor_a + glm::vec2(std::cos(lower_angle), std::sin(lower_angle)) * marker_size;
        const glm::vec2 upper = world_anchor_a + glm::vec2(std::cos(upper_angle), std::sin(upper_angle)) * marker_size;
        renderer.draw_solid_arc(world_anchor_a, marker_size * 0.8f, upper_angle, lower_angle, glm::vec4(marker_color, 0.5f));
        renderer.draw_line(world_anchor_a, lower, marker_width, glm::vec4(marker_color, 1.9f));
        renderer.draw_line(world_anchor_a, upper, marker_width, glm::vec4(marker_color, 1.9f));
    }
}

void RevoluteJoint::load(const YAML::Node &node) {
    Joint::load(node);

    if (node["lower_angle"])
        m_lower_angle = node["lower_angle"].as<float>();
    if (node["upper_angle"])
        m_upper_angle = node["upper_angle"].as<float>();
    if (node["limit_enabled"])
        m_limit_enabled = node["limit_enabled"].as<bool>();

    if (node["is_motor"])
        m_is_motor = node["is_motor"].as<bool>();
    if (node["min_motor_speed"])
        m_min_motor_speed = node["min_motor_speed"].as<float>();
    if (node["max_motor_speed"])
        m_max_motor_speed = node["max_motor_speed"].as<float>();
    if (node["max_motor_torque"])
        m_max_motor_torque = node["max_motor_torque"].as<float>();

    ensure_constraints();
}

void RevoluteJoint::save(YAML::Emitter &emitter) const {
    Joint::save(emitter);

    emitter << YAML::Key << "lower_angle" << YAML::Value << m_lower_angle;
    emitter << YAML::Key << "upper_angle" << YAML::Value << m_upper_angle;
    emitter << YAML::Key << "limit_enabled" << YAML::Value << m_limit_enabled;
    emitter << YAML::Key << "is_motor" << YAML::Value << m_is_motor;
    emitter << YAML::Key << "min_motor_speed" << YAML::Value << m_min_motor_speed;
    emitter << YAML::Key << "max_motor_speed" << YAML::Value << m_max_motor_speed;
    emitter << YAML::Key << "max_motor_torque" << YAML::Value << m_max_motor_torque;
}

void RevoluteJoint::ensure_constraints() {
    m_lower_angle = std::fmod(m_lower_angle, 360.f);
    m_upper_angle = std::fmod(m_upper_angle, 360.f);
    m_lower_angle = std::clamp(m_lower_angle, -180.0f, m_upper_angle);
    m_upper_angle = std::clamp(m_upper_angle, m_lower_angle, 180.f);
    m_min_motor_speed = std::min(m_min_motor_speed, m_max_motor_speed);
    m_max_motor_speed = std::max(m_min_motor_speed, m_max_motor_speed);
}

/**
 * A prismatic joint part.
 */

bool PrismaticJoint::show_inspector() {
    bool changed = Joint::show_inspector();

    if (ImGui::CollapsingHeader("Prismatic joint")) {
        changed |= ImGui::DragFloat2("Local axis A", &m_local_axis_a[0], 0.01f);
        ImGui::SetItemTooltip("The local axis of the joint relative to part A (unit vector).");

        changed |= ImGui::Checkbox("Limit enabled", &m_limit_enabled);
        ImGui::SetItemTooltip("Whether the joint has a rotation limit.");
        if (m_limit_enabled) {
            changed |= ImGui::DragFloat("Lower translation (in m)", &m_lower_translation, 0.01f, -INFINITY, m_upper_translation);
            ImGui::SetItemTooltip("The lower translation limit in meters.");
            changed |= ImGui::DragFloat("Upper translation (in m)", &m_upper_translation, 0.01f, m_lower_translation, INFINITY);
            ImGui::SetItemTooltip("The upper translation limit in meters.");
        }

        ensure_constraints();
    }

    return changed;
}

void PrismaticJoint::draw(Renderer2D &renderer, const DrawPartContext &context) {
    if (!should_draw(context))
        return;

    Joint::draw(renderer, context);

    auto *part_a = get_robot()->get_part_by_name(get_part_a());
    if (part_a != nullptr) {
        auto world_anchor_a = get_world_anchor_a();

        // Draw the local axis
        const float angle_a = glm::radians(part_a->get_angle());
        glm::vec2 world_axis_a = part_a->get_position();
        world_axis_a = glm::vec2(std::cos(angle_a) * m_local_axis_a.x - std::sin(angle_a) * m_local_axis_a.y,
                                 std::sin(angle_a) * m_local_axis_a.x + std::cos(angle_a) * m_local_axis_a.y);
        world_axis_a = glm::normalize(world_axis_a);

        constexpr glm::vec4 marker_color = {1.0f, 1.0f, 0.0f, 1.0f};
        constexpr float marker_width = 0.01f;

        if (m_limit_enabled) {
            const glm::vec2 lower = world_anchor_a + world_axis_a * m_lower_translation;
            const glm::vec2 upper = world_anchor_a + world_axis_a * m_upper_translation;
            renderer.draw_line(lower, upper, marker_width, marker_color);
        } else {
            renderer.draw_infinite_line(world_anchor_a, world_axis_a, marker_width, marker_color);
        }
    }
}

void PrismaticJoint::load(const YAML::Node &node) {
    Joint::load(node);

    if (node["local_axis_a"])
        from_yaml(node["local_axis_a"], m_local_axis_a);

    if (node["lower_translation"])
        m_lower_translation = node["lower_translation"].as<float>();
    if (node["upper_translation"])
        m_upper_translation = node["upper_translation"].as<float>();
    if (node["limit_enabled"])
        m_limit_enabled = node["limit_enabled"].as<bool>();

    if (node["is_motor"])
        m_is_motor = node["is_motor"].as<bool>();
    if (node["min_motor_speed"])
        m_min_motor_speed = node["min_motor_speed"].as<float>();
    if (node["max_motor_speed"])
        m_max_motor_speed = node["max_motor_speed"].as<float>();
    if (node["max_motor_force"])
        m_max_motor_force = node["max_motor_force"].as<float>();

    ensure_constraints();
}

void PrismaticJoint::save(YAML::Emitter &emitter) const {
    Joint::save(emitter);

    emitter << YAML::Key << "local_axis_a" << YAML::Value;
    to_yaml(emitter, m_local_axis_a);
    emitter << YAML::Key << "lower_translation" << YAML::Value << m_lower_translation;
    emitter << YAML::Key << "upper_translation" << YAML::Value << m_upper_translation;
    emitter << YAML::Key << "limit_enabled" << YAML::Value << m_limit_enabled;
    emitter << YAML::Key << "is_motor" << YAML::Value << m_is_motor;
    emitter << YAML::Key << "min_motor_speed" << YAML::Value << m_min_motor_speed;
    emitter << YAML::Key << "max_motor_speed" << YAML::Value << m_max_motor_speed;
    emitter << YAML::Key << "max_motor_force" << YAML::Value << m_max_motor_force;
}

void PrismaticJoint::ensure_constraints() {
    m_local_axis_a = glm::normalize(m_local_axis_a);

    m_lower_translation = std::min(m_lower_translation, m_upper_translation);
    m_upper_translation = std::max(m_lower_translation, m_upper_translation);
    m_min_motor_speed = std::min(m_min_motor_speed, m_max_motor_speed);
    m_max_motor_speed = std::max(m_min_motor_speed, m_max_motor_speed);
}
