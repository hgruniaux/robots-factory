#include "robot_parser.hpp"

#include "parts/joint.hpp"
#include "parts/sensor.hpp"
#include "parts/shape.hpp"

#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

// Parse the 'meta' section of the Robot YAML document.
static void parse_robot_meta(const YAML::Node &meta, const std::shared_ptr<Robot> &robot) {
    robot->set_name(meta["name"].as<std::string>());
}

// Parse a node of the 'parts' section of the Robot YAML document.
static std::unique_ptr<Part> parse_robot_part(const YAML::Node &yaml_node) {
    auto kind = yaml_node["kind"].as<std::string>();
    auto *part_meta = PartManager::get_part_meta_info(kind);
    if (part_meta == nullptr)
        throw YAML::RepresentationException(yaml_node["kind"].Mark(), "unknown node kind '" + kind + "'");

    auto part = part_meta->create_function();
    part->load(yaml_node);
    return part;
}

// Parse the 'parts' section of the Robot YAML document.
static void parse_robot_parts(const YAML::Node &parts, const std::shared_ptr<Robot> &robot) {
    for (const auto &part: parts) {
        auto node = parse_robot_part(part);
        robot->add_part(std::move(node));
    }
}

// Parse the root of the Robot YAML document.
static void parse_robot(const YAML::Node &root, const std::shared_ptr<Robot> &robot) {
    parse_robot_meta(root["meta"], robot);
    parse_robot_parts(root["parts"], robot);
}

std::shared_ptr<Robot> parse_robot(const std::string &path) {
    SPDLOG_TRACE("Loading robot at '{}'...", path);
    try {
        YAML::Node root = YAML::LoadFile(path);
        auto robot = std::make_shared<Robot>();
        robot->set_source_file(path);
        parse_robot(root, robot);

        if (robot != nullptr)
            SPDLOG_TRACE("Successfully loaded robot '{}' at '{}'", robot->get_name(), path);

        return robot;
    } catch (const YAML::Exception &err) {
        if (err.mark.is_null())
            SPDLOG_ERROR("Failed to load robot description\n{}: {}", path, err.what());
        else
            SPDLOG_ERROR("Failed to load robot description\n{}:{}:{}: {}", path, err.mark.line, err.mark.column, err.what());
        return nullptr;
    }
}
