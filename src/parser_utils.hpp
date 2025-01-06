#pragma once

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

inline void from_yaml(const YAML::Node &node, glm::vec2 &v) {
    if (node.IsMap()) {
        v.x = node["x"].as<float>();
        v.y = node["y"].as<float>();
        return;
    } else if (node.IsSequence()) {
        v.x = node[0].as<float>();
        v.y = node[1].as<float>();
    } else {
        throw YAML::RepresentationException(node.Mark(), "Expected a vec2");
    }
}

inline void to_yaml(YAML::Emitter &emitter, const glm::vec2 &v) {
    emitter << YAML::Flow;
    emitter << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
}

inline void from_yaml(const YAML::Node &node, glm::vec3 &v) {
    if (node.IsMap()) {
        v.x = node["x"].as<float>();
        v.y = node["y"].as<float>();
        v.z = node["z"].as<float>();
        return;
    } else if (node.IsSequence()) {
        v.x = node[0].as<float>();
        v.y = node[1].as<float>();
        v.z = node[2].as<float>();
    } else {
        throw YAML::RepresentationException(node.Mark(), "Expected a vec3");
    }
}

inline void to_yaml(YAML::Emitter &emitter, const glm::vec3 &v) {
    emitter << YAML::Flow;
    emitter << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
}

inline void from_yaml(const YAML::Node &node, glm::vec4 &v) {
    if (node.IsMap()) {
        v.x = node["x"].as<float>();
        v.y = node["y"].as<float>();
        v.z = node["z"].as<float>();
        v.w = node["w"].as<float>();
        return;
    } else if (node.IsSequence()) {
        v.x = node[0].as<float>();
        v.y = node[1].as<float>();
        v.z = node[2].as<float>();
        v.w = node[3].as<float>();
    } else {
        throw YAML::RepresentationException(node.Mark(), "Expected a vec4");
    }
}

inline void to_yaml(YAML::Emitter &emitter, const glm::vec4 &v) {
    emitter << YAML::Flow;
    emitter << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
}
