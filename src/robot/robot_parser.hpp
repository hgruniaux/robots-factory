#pragma once

#include "robot.hpp"

Part* parse_robot_part(const nlohmann::json &json_object, const std::shared_ptr<Robot> &robot);

/**
 * Parse a robot description from a YAML file.
 */
std::shared_ptr<Robot> parse_robot(const std::string &path);
