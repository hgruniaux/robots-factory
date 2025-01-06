#pragma once

#include "parts/part.hpp"
#include <box2d/box2d.h>
#include <string>
#include <unordered_map>

class Robot : public std::enable_shared_from_this<Robot> {
public:
    Robot();

    // The source file path (a YAML document) that describes the robot.
    [[nodiscard]] const std::string &get_source_file() const { return m_source_file; }
    void set_source_file(const std::string &source_file) { m_source_file = source_file; }

    // The display name of the robot (in UTF-8).
    [[nodiscard]] const std::string &get_name() const { return m_name; }
    void set_name(const std::string &name) { m_name = name; }

    // The parts of the robot.
    [[nodiscard]] const std::vector<std::unique_ptr<Part>> &get_parts() const { return m_parts; }
    void add_part(std::unique_ptr<Part> part);
    void add_part(Part *part) { add_part(std::unique_ptr<Part>(part)); }
    void insert_part(int index, std::unique_ptr<Part> part);
    void insert_part(int index, Part *part) { insert_part(index, std::unique_ptr<Part>(part)); }
    void remove_part(Part *part);

    [[nodiscard]] int get_part_index(Part *part) const;
    void swap_parts(int from_index, int to_index);

    // The ground part of the robot (if any).
    // The ground is a symbolic part that represents the world ground to where
    // the robot may be attached to (e.g. an articulated arm fixed to the ground).
    [[nodiscard]] const Ground *get_ground_part() const { return m_ground_part.get(); }

    // Retrieve some part of the robot by its unique name.
    [[nodiscard]] Part *get_part_by_name(const std::string &name) const;
    template<class T>
    [[nodiscard]] T *get_part_by_name(const std::string &name) const { return dynamic_cast<T *>(get_part_by_name(name)); }

    // Generate a new unique part name based on the kind name (to be used by the UI).
    [[nodiscard]] std::string get_new_part_name(const std::string &kind_name) const;

private:
    friend class Part;
    void update_part_name(const std::string &old_name, const std::string &new_name);

private:
    std::string m_source_file;
    std::string m_name;
    std::unique_ptr<Ground> m_ground_part;// special symbolic part representing the world ground
    std::vector<std::unique_ptr<Part>> m_parts;
    std::unordered_map<std::string, Part *> m_parts_by_name;
};// class Robot