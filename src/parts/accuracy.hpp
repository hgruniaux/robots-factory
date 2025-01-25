#pragma once

#include <imgui.h>
#include <random>
#include <nlohmann/json.hpp>

struct AccuracyInfo {
    // The accuracy of the sensor (in whatever units the sensor is).
    // A larger accuracy means a higher potential bias.
    float accuracy = 1.0f;
    // The resolution of the sensor (in whatever units the sensor is).
    // Smallest detectable change in the sensor value.
    float resolution = 0.1f;
    // Standard deviation of the random noise.
    float sigma = 0.05f;

    bool show_inspector() {
        bool changed = false;
        if (ImGui::CollapsingHeader("Accuracy info")) {
            changed |= ImGui::DragFloat("Accuracy", &accuracy, 0.01f, 0.0f, INFINITY);
            ImGui::SetItemTooltip("The accuracy of the sensor (in whatever units the sensor is). A larger accuracy means a higher potential bias.");
            changed |= ImGui::DragFloat("Resolution", &resolution, 0.01f, 0.0f, INFINITY);
            ImGui::SetItemTooltip("The resolution of the sensor (in whatever units the sensor is). Smallest detectable change in the sensor value.");
            changed |= ImGui::DragFloat("Sigma", &sigma, 0.01f, 0.0f, INFINITY);
            ImGui::SetItemTooltip("Standard deviation of the random noise.");
        }
        return changed;
    }

    [[nodiscard]] float apply(float true_value) const {
        static std::default_random_engine rd(std::random_device{}());

        float n_bias = std::normal_distribution<float>(0.f, accuracy / 2.f)(rd);
        float n_quantization = std::uniform_real_distribution<float>(-resolution / 2.f, resolution / 2.f)(rd);
        float n_random = std::normal_distribution<float>(0.f, sigma)(rd);
        return true_value + n_bias + n_quantization + n_random;
    }
};

static void to_json(nlohmann::json &j, const AccuracyInfo &v) {
    j = nlohmann::json{
        {"accuracy", v.accuracy},
        {"resolution", v.resolution},
        {"sigma", v.sigma}
    };
}

static void from_json(const nlohmann::json &j, AccuracyInfo &v) {
    j.at("accuracy").get_to(v.accuracy);
    j.at("resolution").get_to(v.resolution);
    j.at("sigma").get_to(v.sigma);
}
