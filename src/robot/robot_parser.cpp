#include "robot_parser.hpp"

#include "parts/part.hpp"

#include <fstream>
#include <spdlog/spdlog.h>

// Parse a node of the 'parts' section of the Robot YAML document.
static std::unique_ptr<Part> parse_robot_part(const nlohmann::json &yaml_node) {
    std::string kind = yaml_node["kind"];
    auto *part_meta = PartManager::get_part_meta_info(kind);
    assert(part_meta != nullptr);

    auto part = part_meta->create_function();
    part->load(yaml_node);
    return part;
}

// Parse the 'parts' section of the Robot YAML document.
static void parse_robot_parts(const nlohmann::json &parts, const std::shared_ptr<Robot> &robot) {
    for (const auto &part: parts) {
        auto node = parse_robot_part(part);
        robot->add_part(std::move(node));
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
