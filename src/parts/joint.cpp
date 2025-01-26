#include "joint.hpp"
#include "draw/renderer_2d.hpp"
#include "parser_utils.hpp"
#include "robot/robot.hpp"
#include "eui.hpp"

/**
 * A joint part.
 */

bool Joint::show_inspector() {
    bool changed = Part::show_inspector();

    if (ImGui::CollapsingHeader("Joint")) {
        changed |= EUI::InputText("Part A", m_part_a);
        ImGui::SetItemTooltip("The name of the first part to connect.");

        changed |= EUI::InputText("Part B", m_part_b);
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

        changed |= EUI::InputVector("Local anchor A", m_local_anchor_a, 0.01f, 0.0f, "%.2f m");
        ImGui::SetItemTooltip("The local anchor point relative to part A center in meters.");
        changed |= EUI::InputVector("Local anchor B", m_local_anchor_b, 0.01f, 0.0f, "%.2f m");
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

void Joint::load(const nlohmann::json &object) {
    Part::load(object);

    m_part_a = object.value("part_a", m_part_a);
    m_part_b = object.value("part_b", m_part_b);

    m_local_anchor_a = object.value("local_anchor_a", m_local_anchor_a);
    m_local_anchor_b = object.value("local_anchor_b", m_local_anchor_b);

    m_should_collide = object.value("should_collide", m_should_collide);

    if (object.contains("accuracy_info")) {
        m_has_accuracy_info = true;
        m_accuracy_info = object["accuracy_info"];
    }
}

void Joint::save(nlohmann::json &object) const {
    Part::save(object);

    object["part_a"] = m_part_a;
    object["part_b"] = m_part_b;

    object["local_anchor_a"] = m_local_anchor_a;
    object["local_anchor_b"] = m_local_anchor_b;

    object["should_collide"] = m_should_collide;

    if (m_has_accuracy_info)
        object["accuracy_info"] = m_accuracy_info;
}

/*
 * The DistanceJoint class
 */


bool DistanceJoint::show_inspector() {
    bool changed = Joint::show_inspector();

    if (ImGui::CollapsingHeader("Distance joint")) {
        changed |= EUI::InputFloat("Length", m_length, 0.01f, m_min_length, m_max_length, "%.2f m");
        ImGui::SetItemTooltip("The desired length of the joint at rest in meters.");
        changed |= EUI::InputFloat("Min length", m_min_length, 0.01f, 0.f, m_max_length, "%.2f m");
        ImGui::SetItemTooltip("The minimum length of the joint in meters.");
        changed |= EUI::InputFloat("Max length", m_max_length, 0.01f, m_min_length, INFINITY, "%.2f m");
        ImGui::SetItemTooltip("The maximum length of the joint in meters.");
        changed |= EUI::InputFloat("Frequency", m_frequency, 1.f, 0.f, 30.f, "%.2f Hz");
        ImGui::SetItemTooltip("The frequency of the spring in Hz.");
        changed |= EUI::InputFloat("Damping ratio", m_damping_ratio, 0.01f, 0.0f, 1.0f);
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

void DistanceJoint::load(const nlohmann::json &object) {
    Joint::load(object);

    m_length = object.value("length", m_length);
    m_min_length = object.value("min_length", m_min_length);
    m_max_length = object.value("max_length", m_max_length);
    if (m_max_length >= 1e30f) // JSON doesn't support inf
        m_max_length = std::numeric_limits<float>::infinity();
    m_frequency = object.value("frequency", m_frequency);
    m_damping_ratio = object.value("damping_ratio", m_damping_ratio);

    ensure_constraints();
}

void DistanceJoint::save(nlohmann::json &object) const {
    Joint::save(object);

    object["length"] = m_length;
    object["min_length"] = m_min_length;

    if (m_max_length == std::numeric_limits<float>::infinity()) // JSON doesn't support inf
        object["max_length"] = std::numeric_limits<float>::max();
    else
        object["max_length"] = m_max_length;

    object["frequency"] = m_frequency;
    object["damping_ratio"] = m_damping_ratio;
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
            changed |= EUI::InputAngle("Lower angle", m_lower_angle, 1.0f, -180.f, m_upper_angle);
            ImGui::SetItemTooltip("The lower angle limit in degrees.");
            changed |= EUI::InputAngle("Upper angle", m_upper_angle, 1.0f, m_lower_angle, 180.f);
            ImGui::SetItemTooltip("The upper angle limit in degrees.");
        }

        changed |= ImGui::Checkbox("Is motor", &m_is_motor);
        ImGui::SetItemTooltip("Whether the joint is a motor.");
        if (m_is_motor) {
            changed |= EUI::InputFloat("Min motor speed", m_min_motor_speed, 1.0f, 0.f, m_max_motor_speed, "%.2f °/s");
            ImGui::SetItemTooltip("The minimum motor speed in degrees per second.");
            changed |= EUI::InputFloat("Max motor speed", m_max_motor_speed, 1.0f, m_min_motor_speed, INFINITY, "%.2f °/s");
            ImGui::SetItemTooltip("The maximum motor speed in degrees per second.");
            changed |= EUI::InputFloat("Max motor torque", m_max_motor_torque, 0.1f, 0.f, INFINITY, "%.2f N*m");
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

void RevoluteJoint::load(const nlohmann::json &object) {
    Joint::load(object);

    m_lower_angle = object.value("lower_angle", m_lower_angle);
    m_upper_angle = object.value("upper_angle", m_upper_angle);
    m_limit_enabled = object.value("limit_enabled", m_limit_enabled);

    m_is_motor = object.value("is_motor", m_is_motor);
    m_min_motor_speed = object.value("min_motor_speed", m_min_motor_speed);
    m_max_motor_speed = object.value("max_motor_speed", m_max_motor_speed);
    m_max_motor_torque = object.value("max_motor_torque", m_max_motor_torque);

    ensure_constraints();
}

void RevoluteJoint::save(nlohmann::json &object) const {
    Joint::save(object);

    object["lower_angle"] = m_lower_angle;
    object["upper_angle"] = m_upper_angle;
    object["limit_enabled"] = m_limit_enabled;
    object["is_motor"] = m_is_motor;
    object["min_motor_speed"] = m_min_motor_speed;
    object["max_motor_speed"] = m_max_motor_speed;
    object["max_motor_torque"] = m_max_motor_torque;
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
        changed |= EUI::InputVector("Local axis A", m_local_axis_a, 0.01f);
        ImGui::SetItemTooltip("The local axis of the joint relative to part A (unit vector).");

        changed |= ImGui::Checkbox("Limit enabled", &m_limit_enabled);
        ImGui::SetItemTooltip("Whether the joint has a rotation limit.");
        if (m_limit_enabled) {
            changed |= EUI::InputFloat("Lower translation", m_lower_translation, 0.01f, -INFINITY, m_upper_translation, "%.2f m");
            ImGui::SetItemTooltip("The lower translation limit in meters.");
            changed |= EUI::InputFloat("Upper translation", m_upper_translation, 0.01f, m_lower_translation, INFINITY, "%.2f m");
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

void PrismaticJoint::load(const nlohmann::json &object) {
    Joint::load(object);

    m_local_axis_a = object.value("local_axis_a", m_local_axis_a);

    m_lower_translation = object.value("lower_translation", m_lower_translation);
    m_upper_translation = object.value("upper_translation", m_upper_translation);
    m_limit_enabled = object.value("limit_enabled", m_limit_enabled);

    m_is_motor = object.value("is_motor", m_is_motor);
    m_min_motor_speed = object.value("min_motor_speed", m_min_motor_speed);
    m_max_motor_speed = object.value("max_motor_speed", m_max_motor_speed);
    m_max_motor_force = object.value("max_motor_force", m_max_motor_force);

    ensure_constraints();
}

void PrismaticJoint::save(nlohmann::json &object) const {
    Joint::save(object);

    object["local_axis_a"] = m_local_axis_a;
    object["lower_translation"] = m_lower_translation;
    object["upper_translation"] = m_upper_translation;
    object["limit_enabled"] = m_limit_enabled;
    object["is_motor"] = m_is_motor;
    object["min_motor_speed"] = m_min_motor_speed;
    object["max_motor_speed"] = m_max_motor_speed;
    object["max_motor_force"] = m_max_motor_force;
}

void PrismaticJoint::ensure_constraints() {
    m_local_axis_a = glm::normalize(m_local_axis_a);

    m_lower_translation = std::min(m_lower_translation, m_upper_translation);
    m_upper_translation = std::max(m_lower_translation, m_upper_translation);
    m_min_motor_speed = std::min(m_min_motor_speed, m_max_motor_speed);
    m_max_motor_speed = std::max(m_min_motor_speed, m_max_motor_speed);
}
