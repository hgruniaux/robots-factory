#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "IconsFontAwesome6.h"
#include "part_manager.hpp"

class Robot;
class Renderer2D;
class Part;
class FolderPart;

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

    template<typename T>
    [[nodiscard]] bool is() const { return dynamic_cast<const T *>(this) != nullptr; }
    template<typename T>
    [[nodiscard]] T *as() { return dynamic_cast<T *>(this); }
    template<typename T>
    [[nodiscard]] const T *as() const { return dynamic_cast<const T *>(this); }

    // The owner robot of the part.
    [[nodiscard]] const std::shared_ptr<Robot> &get_robot() const { return m_robot; }

    // The parent part of the part (when inside a folder part).
    [[nodiscard]] Part* get_parent_part() const { return m_parent_part; }

    // The folder part that contains this part (or nullptr).
    [[nodiscard]] FolderPart* get_folder_parent() const;
    [[nodiscard]] bool is_inside_folder() const { return get_folder_parent() != nullptr; }

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

    // JSON serialization/deserialization.
    virtual void load(const nlohmann::json &object);
    virtual void save(nlohmann::json &object) const;

protected:
    virtual void notify_new_robot() {}

private:
    friend class Robot;
    friend class FolderPart;

    // The owner robot of the part.
    std::shared_ptr<Robot> m_robot;

    // If the part is inside a folder part, this is the parent part.
    Part* m_parent_part = nullptr;

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

class FolderPart : public Part {
public:
    DECLARE_PART(FolderPart, ICON_FA_FOLDER);

    // The color of the folder.
    [[nodiscard]] const glm::vec3 &get_color() const { return m_color; }
    void set_color(const glm::vec3 &color) { m_color = color; }

    // The parts of the folder.
    [[nodiscard]] const std::vector<Part*>& get_parts() const { return m_parts; }
    void add_part(Part *part);
    void insert_part(int index, Part *part);
    void remove_part(Part *part);
    void clear();

    [[nodiscard]] int get_part_index(Part *part) const;
    void swap_parts(int from_index, int to_index);

    bool show_inspector() override;
    void draw(Renderer2D &renderer, const DrawPartContext &context) override;
    void load(const nlohmann::json &object) override;
    void save(nlohmann::json &object) const override;

private:
    // Parts are owned by Robot, FolderPart only owns references.
    std::vector<Part*> m_parts;
    glm::vec3 m_color = {0.8f, 0.6f, 0.13f}; // orange-yellow
};// class FolderPart
