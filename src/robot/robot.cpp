#include "robot.hpp"
#include "parts/part.hpp"
#include <fmt/format.h>

Robot::Robot() {
    m_ground_part = std::make_unique<Ground>();

    // We cannot call shared_from_this() from the constructor, therefore we cannot set
    // the m_ground_part->m_robot field to this. As this part is quite special, we
    // don't really care...
    // ERROR: m_ground_part->m_robot = shared_from_this();

    // As m_ground_part->m_robot is nullptr, we cannot use register_part_name().
    m_parts_by_name.insert({m_ground_part->get_name(), m_ground_part.get()});
}

void Robot::add_part(std::unique_ptr<Part> part) {
    assert(part != nullptr);
    assert(part->m_robot == nullptr && "part already belongs to a robot");

    part->m_robot = shared_from_this();
    register_part_name(part->get_name(), part.get());
    m_parts.push_back(std::move(part));
}

void Robot::insert_part(int index, std::unique_ptr<Part> part) {
    assert(part != nullptr);
    assert(index >= 0 && index <= m_parts.size() && "index out of bounds");
    assert(part->m_robot == nullptr && "part already belongs to a robot");

    part->m_robot = shared_from_this();
    register_part_name(part->get_name(), part.get());
    m_parts.insert(m_parts.begin() + index, std::move(part));
}

void Robot::remove_part(Part *part) {
    auto* part_ptr = release_part(part);
    delete part_ptr;
}

Part* Robot::release_part(Part *part) {
    assert(part != nullptr);
    assert(part->m_robot.get() == this && "part must belong to this robot");

    unregister_part_name(part->get_name());

    // Special case if inside a folder.
    auto* folder_parent = part->get_folder_parent();
    if (folder_parent != nullptr)
        folder_parent->remove_part(part);

    // Remove part from the robot global table.
    auto it = std::find_if(m_parts.begin(), m_parts.end(), [part](const auto &p) { return p.get() == part; });
    assert(it != m_parts.end() && "part not found in robot");
    auto* part_ptr = it->release();
    m_parts.erase(it);
    part_ptr->m_robot = nullptr;
    return part_ptr;
}

int Robot::get_part_index(Part *part) const {
    const auto it = std::find_if(m_parts.begin(), m_parts.end(), [part](const auto &p) { return p.get() == part; });
    assert(it != m_parts.end() && "part not found in robot");
    return (int) std::distance(m_parts.begin(), it);
}

void Robot::swap_parts(int from_index, int to_index) {
    assert(from_index >= 0 && from_index < m_parts.size());
    assert(to_index >= 0 && to_index < m_parts.size());
    std::swap(m_parts[from_index], m_parts[to_index]);
}

Part *Robot::get_part_by_name(const std::string &name) const {
    const auto it = m_parts_by_name.find(name);
    if (it == m_parts_by_name.end())
        return nullptr;
    return it->second;
}

std::string Robot::get_unique_part_name(const std::string &kind_name) const {
    std::string name = kind_name;
    int i = 1;
    while (!is_part_name_available(name)) // name already exists
        name = fmt::format("{} {}", kind_name, i++);
    return name;
}

bool Robot::is_part_name_available(const std::string &name) const {
    return m_parts_by_name.find(name) == m_parts_by_name.end();
}

void Robot::register_part_name(const std::string &name, Part *part) {
    assert(part->m_robot.get() == this && "part must belong to this robot");
    if (name.empty())
        return; // skip empty names
    auto [_, inserted] = m_parts_by_name.insert({name, part});
    assert(inserted && "part name must be unique");
}

void Robot::unregister_part_name(const std::string &name) {
    if (name.empty())
        return; // skip empty names
    bool removed = m_parts_by_name.erase(name) > 0;
    assert(removed && "part name must exist");
}

void Robot::update_part_name(const std::string &old_name, Part *part) {
    assert(part != nullptr);

    if (old_name == part->get_name())
        return; // early exit for trivial case

    unregister_part_name(old_name);
    register_part_name(part->get_name(), part);
}
