#include "robot_parser.hpp"

#include "parts/part.hpp"

#include <fstream>
#include <spdlog/spdlog.h>

// Parse a node of the 'parts' section of the Robot YAML document.
Part* parse_robot_part(const nlohmann::json &json_object, const std::shared_ptr<Robot> &robot) {
    std::string kind = json_object["kind"];
    auto *part_meta = PartManager::get_part_meta_info(kind);
    assert(part_meta != nullptr);

    auto part = part_meta->create_function();
    auto* part_ptr = part.get();
    robot->add_part(std::move(part));
    part_ptr->load(json_object);
    return part_ptr;
}

// Parse the 'parts' section of the Robot YAML document.
static void parse_robot_parts(const nlohmann::json &parts, const std::shared_ptr<Robot> &robot) {
    for (const auto &part: parts) {
        parse_robot_part(part, robot);
    }
}

// Parse the root of the Robot YAML document.
static void parse_robot(const nlohmann::json &root, const std::shared_ptr<Robot> &robot) {
    robot->set_name(root.at("name"));
    parse_robot_parts(root.at("parts"), robot);
}

std::shared_ptr<Robot> parse_robot(const std::string &path) {
    SPDLOG_TRACE("Loading robot at '{}'...", path);

    nlohmann::json root;
    std::ifstream stream(path);
    if (!stream.is_open()) {
        SPDLOG_ERROR("Failed to open file '{}'", path);
        return nullptr;
    }

    try {
        stream >> root;

        auto robot = std::make_shared<Robot>();
        robot->set_source_file(path);
        parse_robot(root, robot);

        if (robot != nullptr)
            SPDLOG_TRACE("Successfully loaded robot '{}' at '{}'", robot->get_name(), path);

        return robot;
    } catch (const nlohmann::json::exception &e) {
        SPDLOG_ERROR("Failed to parse JSON:\n{}", e.what());
        return nullptr;
    }
}
