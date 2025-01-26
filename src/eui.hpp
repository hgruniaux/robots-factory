// Copyright (c) 2025 Hubert Gruniaux
// This file is part of orchid which is released under the MIT license.
// See file LICENSE.txt for full license details.

#ifndef EUI_HPP
#define EUI_HPP

#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <string>

namespace EUI {
bool Begin(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0);
void End();

static constexpr float DEFAULT_COLUMN_WIDTH = 125.0f;

template <class F>
static bool InputHelper(const char* label, float column_width, F&& f)
{
    ImGui::PushID(label);
    ImGui::BeginTable(label, 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoPadOuterX);
    ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthFixed, column_width);
    ImGui::TableSetupColumn("##Content", ImGuiTableColumnFlags_WidthStretch);
    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::TextWrapped(label);
    ImGui::TableNextColumn();
    ImGui::SetNextItemWidth(-1);
    bool modified = f();
    ImGui::EndTable();
    ImGui::PopID();
    return modified;
}

bool InputCheckbox(const char* label, bool& value, float column_width = DEFAULT_COLUMN_WIDTH);
bool InputFloat(const char* label, float& value, float v_step = 1.0f, const char* format = "%.2f", float column_width = DEFAULT_COLUMN_WIDTH);
bool InputFloat(const char* label, float& value, float v_step, float min_width, float max_width, const char* format = "%.2f", float column_width = DEFAULT_COLUMN_WIDTH);
bool InputAngle(const char* label, float& value, float v_step = 1.0f, float column_width = DEFAULT_COLUMN_WIDTH);
bool InputAngle(const char* label, float& value, float v_step, float min_angle, float max_angle, float column_width = DEFAULT_COLUMN_WIDTH);
bool InputVector(const char* label, glm::vec2& value, float v_step = 1.0f, float reset_value = 0.0f, const char* format = "%.2f", float column_width = DEFAULT_COLUMN_WIDTH);
bool InputVector(const char* label, glm::vec3& value, float v_step = 1.0f, float reset_value = 0.0f, const char* format = "%.2f", float column_width = DEFAULT_COLUMN_WIDTH);
bool InputVector(const char* label, glm::vec4& value, float v_step = 1.0f, float reset_value = 0.0f, const char* format = "%.2f", float column_width = DEFAULT_COLUMN_WIDTH);
bool InputColor(const char* label, glm::vec3& value, float column_width = DEFAULT_COLUMN_WIDTH);
bool InputColor(const char* label, glm::vec4& value, float column_width = DEFAULT_COLUMN_WIDTH);
bool InputText(const char* label, std::string& value, ImGuiInputTextFlags flags = 0, float column_width = DEFAULT_COLUMN_WIDTH);
bool InputTextMultiline(const char* label, std::string& value, ImGuiInputTextFlags flags = 0, float column_width = DEFAULT_COLUMN_WIDTH);
} // namespace EUI

#endif // EUI_HPP
