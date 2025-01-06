#include "robot_saver.hpp"

#include <fstream>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

void save_robot_meta(YAML::Emitter &emitter, const std::shared_ptr<Robot> &robot) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "name" << YAML::Value << robot->get_name();
    emitter << YAML::EndMap;
}

void save_robot_part(YAML::Emitter &emitter, const Part *part) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "kind" << YAML::Value << part->get_kind_name();
    part->save(emitter);
    emitter << YAML::EndMap;
}

void save_robot(const std::shared_ptr<Robot> &robot, const std::string &path) {
    SPDLOG_TRACE("Saving robot '{}' to '{}'", robot->get_name(), path);

    YAML::Emitter emitter;

    emitter << YAML::BeginMap;
    emitter << YAML::Key << "meta" << YAML::Value;
    save_robot_meta(emitter, robot);
    emitter << YAML::Key << "parts" << YAML::Value << YAML::BeginSeq;
    for (const auto &part: robot->get_parts())
        save_robot_part(emitter, part.get());
    emitter << YAML::EndSeq << YAML::EndMap;

    std::ofstream file(path);
    file << emitter.c_str();
}

void save_robot(const std::shared_ptr<Robot> &robot) {
    save_robot(robot, robot->get_source_file());
}
