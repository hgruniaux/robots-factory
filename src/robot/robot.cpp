#include "robot.hpp"
#include <fmt/format.h>

Robot::Robot() {

    m_ground_part = std::make_unique<Ground>();
    m_ground_part->m_name = "@ground@";
    m_parts_by_name[m_ground_part->m_name] = m_ground_part.get();
}

void Robot::add_part(std::unique_ptr<Part> part) {
    assert(part != nullptr);
    part->m_robot = shared_from_this();
    m_parts.push_back(std::move(part));
    m_parts_by_name.insert({m_parts.back()->get_name(), m_parts.back().get()});
}

void Robot::insert_part(int index, std::unique_ptr<Part> part) {
    assert(part != nullptr);
    part->m_robot = shared_from_this();
    m_parts.insert(m_parts.begin() + index, std::move(part));
    m_parts_by_name.insert({m_parts[index]->get_name(), m_parts[index].get()});
}

void Robot::remove_part(Part *part) {
    assert(part != nullptr && part->m_robot.get() == this);
    m_parts_by_name.erase(part->get_name());
    m_parts.erase(std::remove_if(m_parts.begin(), m_parts.end(), [part](const auto &p) { return p.get() == part; }), m_parts.end());
}

int Robot::get_part_index(Part *part) const {
    const auto it = std::find_if(m_parts.begin(), m_parts.end(), [part](const auto &p) { return p.get() == part; });
    if (it == m_parts.end())
        return -1;
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

std::string Robot::get_new_part_name(const std::string &kind_name) const {
    std::string name = kind_name;
    int i = 1;
    while (get_part_by_name(name) != nullptr) {
        name = fmt::format("{} {}", kind_name, i++);
    }
    return name;
}

void Robot::update_part_name(const std::string &old_name, const std::string &new_name) {
    if (old_name == new_name)
        return;

    const auto it = m_parts_by_name.find(old_name);
    if (it == m_parts_by_name.end())
        return;

    Part *part = it->second;
    m_parts_by_name.erase(it);
    m_parts_by_name.insert({new_name, part});
}
