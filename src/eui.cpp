// Copyright (c) 2025 Hubert Gruniaux
// This file is part of orchid which is released under the MIT license.
// See file LICENSE.txt for full license details.

#include "eui.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

bool EUI::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
    if (!ImGui::Begin(name, p_open, flags)) {
        ImGui::End();
        return false;
    }

    return true;
}

void EUI::End()
{
    ImGui::End();
}

bool EUI::InputCheckbox(const char* label, bool& value, float column_width)
{
    return InputHelper(label, column_width, [&value]() {
        return ImGui::Checkbox("##Value", &value);
    });
}

bool EUI::InputFloat(const char* label, float& value, float v_step, const char* format, float column_width)
{
    return InputHelper(label, column_width, [&value, v_step, format]() {
        return ImGui::DragFloat("##Value", &value, v_step, 0.0f, 0.0f, format);
    });
}

bool EUI::InputFloat(const char* label, float& value, float v_step, float min_val, float max_val, const char* format, float column_width)
{
    return InputHelper(label, column_width, [&value, v_step, min_val, max_val, format]() {
        return ImGui::DragFloat("##Value", &value, v_step, min_val, max_val, format);
    });
}

bool EUI::InputAngle(const char* label, float& value, float v_step, float column_width)
{
    return InputHelper(label, column_width, [&value, v_step]() {
        bool modified = ImGui::DragFloat("##Value", &value, v_step, 0.0f, 0.0f, "%.2f°");
        value = glm::mod(value, 360.0f);
        return modified;
    });
}

bool EUI::InputAngle(const char* label, float& value, float v_step, float min_angle, float max_angle, float column_width)
{
    return InputHelper(label, column_width, [&value, v_step, min_angle, max_angle]() {
        bool modified = ImGui::DragFloat("##Value", &value, v_step, min_angle, max_angle, "%.2f°");
        value = std::fmod(value, max_angle - min_angle);
        return modified;
    });
}

static bool InputVector(const char* label, float* values, int n_values, float v_step, float reset_value, const char* format, float column_width)
{
    return EUI::InputHelper(label, column_width, [&values, n_values, v_step, reset_value, format]() {
        // Overview of the UI:
        // Label: [X][Drag float][Y][Drag float]

        static constexpr ImVec4 button_colors[] = {
            ImVec4(0.8f, 0.1f, 0.15f, 1.0f), // X
            ImVec4(0.2f, 0.7f, 0.2f, 1.0f), // Y
            ImVec4(0.2f, 0.2f, 0.8f, 1.0f), // Z
            ImVec4(0.7f, 0.1f, 0.7f, 1.0f), // W
        };

        static constexpr ImVec4 button_hovered_colors[] = {
            ImVec4(0.9f, 0.2f, 0.2f, 1.0f), // X
            ImVec4(0.3f, 0.8f, 0.3f, 1.0f), // Y
            ImVec4(0.3f, 0.3f, 0.9f, 1.0f), // Z
            ImVec4(0.8f, 0.1f, 0.8f, 1.0f), // W
        };

        static constexpr ImVec4 button_active_colors[] = {
            ImVec4(0.7f, 0.05f, 0.1f, 1.0f), // X
            ImVec4(0.1f, 0.6f, 0.1f, 1.0f), // Y
            ImVec4(0.1f, 0.1f, 0.7f, 1.0f), // Z
            ImVec4(0.6f, 0.05f, 0.6f, 1.0f), // W
        };

        static constexpr const char* labels[] = { "X", "Y", "Z", "W" };
        static const char* ids[] = { "##X", "##Y", "##Z", "##W" };

        bool modified = false;

        const float line_height = ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f;
        const ImVec2 button_size = ImVec2(line_height + 3.0f, line_height);

        // Compute the width of inputs taking into account the buttons
        const float button_width = button_size.x * n_values;
        const float width_full = ImGui::CalcItemWidth();
        const float input_width = (width_full - button_width) / n_values;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

        for (int i = 0; i < n_values; i++) {
            ImGui::PushStyleColor(ImGuiCol_Button, button_colors[i]);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hovered_colors[i]);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_colors[i]);
            if (ImGui::Button(labels[i], button_size))
                values[i] = reset_value;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(input_width);
            modified |= ImGui::DragFloat(ids[i], &values[i], v_step, 0.0f, 0.0f, format);
            ImGui::PopItemWidth();
            if (i < n_values - 1)
                ImGui::SameLine();
        }

        ImGui::PopStyleVar();

        return modified;
    });
}

bool EUI::InputVector(const char* label, glm::vec2& value, float v_step, float reset_value, const char* format, float column_width)
{
    return ::InputVector(label, &value.x, 2, v_step, reset_value, format, column_width);
}

bool EUI::InputVector(const char* label, glm::vec3& value, float v_step, float reset_value, const char* format, float column_width)
{
    return ::InputVector(label, &value.x, 3, v_step, reset_value, format, column_width);
}

bool EUI::InputVector(const char* label, glm::vec4& value, float v_step, float reset_value, const char* format, float column_width)
{
    return ::InputVector(label, &value.x, 4, v_step, reset_value, format, column_width);
}

bool EUI::InputColor(const char* label, glm::vec3& value, float column_width)
{
    return InputHelper(label, column_width, [&value]() {
        if (ImGui::ColorButton("##ColorPicker", ImVec4(value.x, value.y, value.z, 1.0f), ImGuiColorEditFlags_NoAlpha, ImVec2(ImGui::CalcItemWidth(), 0))) {
            ImGui::OpenPopup("ColorPickerPopup");
        }

        bool modified = false;
        if (ImGui::BeginPopup("ColorPickerPopup")) {
            modified |= ImGui::ColorPicker3("##ColorPicker", &value.x);
            ImGui::EndPopup();
        }
        return modified;
    });
}

bool EUI::InputColor(const char* label, glm::vec4& value, float column_width)
{
    return InputHelper(label, column_width, [&value]() {
        if (ImGui::ColorButton("##ColorPicker", ImVec4(value.x, value.y, value.z, value.w), ImGuiColorEditFlags_AlphaPreview, ImVec2(ImGui::CalcItemWidth(), 0))) {
            ImGui::OpenPopup("ColorPickerPopup");
        }

        bool modified = false;
        if (ImGui::BeginPopup("ColorPickerPopup")) {
            modified |= ImGui::ColorPicker4("##ColorPicker", &value.x, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreviewHalf);
            ImGui::EndPopup();
        }
        return modified;
    });
}

bool EUI::InputText(const char* label, std::string& value, ImGuiInputTextFlags flags, float column_width)
{
    return InputHelper(label, column_width, [&value, flags]() {
        return ImGui::InputText("##Value", &value, flags);
    });
}

bool EUI::InputTextMultiline(const char* label, std::string& value, ImGuiInputTextFlags flags, float column_width)
{
    return InputHelper(label, column_width, [&value, flags]() {
        return ImGui::InputTextMultiline("##Value", &value, { 0.0f, 0.0f }, flags);
    });
}
