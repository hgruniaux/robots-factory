#include "robot_saver.hpp"

#include <fstream>
#include <spdlog/spdlog.h>

void save_robot(const std::shared_ptr<Robot> &robot, const std::string &path) {
    SPDLOG_TRACE("Saving robot '{}' to '{}'", robot->get_name(), path);

    nlohmann::json json;
    json["name"] = robot->get_name();

    auto parts = nlohmann::json::array();
    for (const auto &part: robot->get_parts()) {
        nlohmann::json part_json;
        part_json["kind"] = part->get_kind_name();
        part->save(part_json);
        parts.push_back(part_json);
    }
    json["parts"] = parts;

    std::ofstream file(path);
    file << json.dump(4);
}

void save_robot(const std::shared_ptr<Robot> &robot) {
    save_robot(robot, robot->get_source_file());
}
