#pragma once

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace glm {
    static void to_json(json &j, const glm::vec2 &v) {
        j = json::array({v.x, v.y});
    }

    static void from_json(const json &j, glm::vec2 &v) {
        j.at(0).get_to(v.x);
        j.at(1).get_to(v.y);
    }

    static void to_json(json &j, const glm::vec3 &v) {
        j = json::array({v.x, v.y, v.z});
    }

    static void from_json(const json &j, glm::vec3 &v) {
        j.at(0).get_to(v.x);
        j.at(1).get_to(v.y);
        j.at(2).get_to(v.z);
    }

    static void to_json(json &j, const glm::vec4 &v) {
        j = json::array({v.x, v.y, v.z, v.w});
    }

    static void from_json(const json &j, glm::vec4 &v) {
        j.at(0).get_to(v.x);
        j.at(1).get_to(v.y);
        j.at(2).get_to(v.z);
        j.at(3).get_to(v.w);
    }
}
