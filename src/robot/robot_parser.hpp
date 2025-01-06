#pragma once

#include "robot.hpp"

/**
 * Parse a robot description from a YAML file.
 */
std::shared_ptr<Robot> parse_robot(const std::string &path);
