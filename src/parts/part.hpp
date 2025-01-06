#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "IconsFontAwesome6.h"
#include "part_manager.hpp"

class Robot;
class Renderer2D;
class Part;

struct DrawPartContext {
    Part *selected_part = nullptr;
    bool draw_joints = true;
    bool draw_only_selected_joints = true;
};// struct DrawPartContext

/**
 * A part is a node in the robot parts tree.
 */
class Part {
public:
    virtual ~Part() = default;

    // The owner robot of the part.
    [[nodiscard]] const std::shared_ptr<Robot> &get_robot() const { return m_robot; }

    // The name of the node. Should be unique.
    [[nodiscard]] const std::string &get_name() const { return m_name; }
    void set_name(const std::string &name);

    // The part's position (in meters).
    [[nodiscard]] glm::vec2 get_position() const { return m_position; }
    void set_position(const glm::vec2 &position) { m_position = position; }

    // The part's rotation (in degrees).
    [[nodiscard]] float get_angle() const { return m_angle; }
    void set_angle(float angle) { m_angle = angle; }

    // The FontAwesome icon to be used for this part (or empty string).
    [[nodiscard]] virtual const char *get_icon() const = 0;
    // The kind name of the part (like "Led" or "DistanceSensor").
    [[nodiscard]] virtual const char *get_kind_name() const = 0;

    virtual bool show_inspector();
    virtual void draw(Renderer2D &renderer, const DrawPartContext &context);

    // YAML serialization/deserialization.
    virtual void load(const YAML::Node &node);
    virtual void save(YAML::Emitter &emitter) const;

private:
    friend class Robot;

    // The owner robot of the part.
    std::shared_ptr<Robot> m_robot;

    // The name of the part. Should be unique to the robot.
    std::string m_name;

    // The transform of the part.
    glm::vec2 m_position = {0.0f, 0.0f};// in meters
    float m_angle = 0.0f;               // in degrees
};// class Node

#define DECLARE_PART(class_name, icon_name)                                                   \
    static inline auto meta = PartManager::register_part<class_name>(#class_name, icon_name); \
    [[nodiscard]] static const char *get_static_icon() { return icon_name; }                  \
    [[nodiscard]] const char *get_icon() const override { return get_static_icon(); }         \
    [[nodiscard]] static const char *get_static_kind_name() { return #class_name; }           \
    [[nodiscard]] const char *get_kind_name() const override { return get_static_kind_name(); }

/// Special part that represents the ground (a component fixed relatively to the world).
/// This can be used to fix the robot to the ground with a joint.
class Ground : public Part {
public:
    DECLARE_PART(Ground, ICON_FA_GLOBE);

    Ground() { set_name("@ground@"); }
};// class Ground

/**
 * A LED is a node that emits light.
 */
class Led : public Part {
public:
    DECLARE_PART(Led, ICON_FA_LIGHTBULB);

    // The state of the LED (on or off).
    [[nodiscard]] bool is_on() const { return m_state; }
    void set_state(bool state) { m_state = state; }
    void turn_on() { m_state = true; }
    void turn_off() { m_state = false; }

    // The RGB color of the LED (when on).
    [[nodiscard]] glm::vec3 get_color() const { return m_color; }
    void set_color(const glm::vec3 &color) { m_color = color; }

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const YAML::Node &node) override;
    void save(YAML::Emitter &emitter) const override;

private:
    glm::vec3 m_color = {1.0f, 0.0f, 0.0f};
    bool m_state = false;
};// class Led
