#include "robot_inspector_ui.hpp"

#include "IconsFontAwesome6.h"
#include <imgui.h>
#include <imgui_stdlib.h>

#include "parts/joint.hpp"
#include "parts/sensor.hpp"
#include "parts/shape.hpp"
#include "robot_parser.hpp"
#include "robot_saver.hpp"

#include <fmt/format.h>
#include <spdlog/spdlog.h>

RobotInspectorUI::RobotInspectorUI() = default;

RobotInspectorUI::RobotInspectorUI(const std::shared_ptr<Robot> &robot)
    : RobotInspectorUI() {
    set_robot(robot);
}

void RobotInspectorUI::set_robot(const std::shared_ptr<Robot> &robot) {
    m_robot = robot;
    m_selected_node = nullptr;
    m_dirty = false;
    m_delayed_operation.kind = DelayedOperation::None;

    if (m_check_constraints)
        check_constraints();
}

void RobotInspectorUI::show() {
    bool changed = false;

    if (m_show_preview)
        changed |= show_preview();

    changed |= show_robot_inspector();

    if (m_show_part_inspector)
        changed |= show_part_inspector();

    m_dirty |= changed;
    execute_delayed_operation();

    if (changed && m_check_constraints)
        check_constraints();
}

void RobotInspectorUI::reload(bool force) {
    if (m_dirty && !force) {
        ImGui::OpenPopup("Unsaved changes");
        return;
    }

    auto new_robot = parse_robot(m_robot->get_source_file());
    if (new_robot != nullptr) {
        m_robot = new_robot;
        m_selected_node = nullptr;
        m_dirty = false;
    }
}

void RobotInspectorUI::save() {
    m_dirty = false;
    save_robot(m_robot);
}

bool RobotInspectorUI::show_preview() {
    if (ImGui::Begin(ICON_FA_ROBOT " Robot Preview")) {
        m_scene_view.begin();

        if (m_robot != nullptr) {
            DrawPartContext context;
            context.draw_joints = true;
            context.selected_part = m_selected_node;

            for (const auto &part: m_robot->get_parts()) {
                part->draw(m_renderer, context);
            }
        }

        m_scene_view.end();
    }
    ImGui::End();

    return false;
}

bool RobotInspectorUI::show_robot_inspector() {
    if (m_robot == nullptr) {
        ImGui::Begin(ICON_FA_ROBOT " Robot inspector");
        ImGui::Text("No robot loaded...");
        ImGui::End();
        return false;
    }

    bool changed = false;

    if (ImGui::Begin(ICON_FA_ROBOT " Robot inspector")) {
        ImGui::Text("Name:   %s", m_robot->get_name().c_str());
        ImGui::Text("Source: %s", m_robot->get_source_file().c_str());

        if (ImGui::Checkbox("Auto check constraints", &m_check_constraints))
            check_constraints();

        if (ImGui::Button(ICON_FA_FLASK " Check constraints"))
            check_constraints();


        if (!m_robot->get_source_file().empty()) {
            if (ImGui::Button(ICON_FA_ROTATE_RIGHT " Reload"))
                reload();
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_FLOPPY_DISK " Save"))
                save();
        }

        if (m_dirty)
            ImGui::TextColored({1.0f, 0.0f, 0.0f, 1.0f}, ICON_FA_TRIANGLE_EXCLAMATION " Unsaved changes");

        changed |= show_part_list();

        show_unsaved_changes_modal();
    }
    ImGui::End();

    return changed;
}

bool RobotInspectorUI::show_part_inspector() {
    bool changed = false;

    if (ImGui::Begin(ICON_FA_PUZZLE_PIECE " Part inspector")) {
        if (m_selected_node) {
            ImGui::PushID(m_selected_node);
            changed = m_selected_node->show_inspector();
            ImGui::PopID();
        } else {
            ImGui::Text("No robot part selected...");
        }
    }
    ImGui::End();

    return changed;
}

bool RobotInspectorUI::show_part_list() {
    bool changed = false;

    ImGui::Text("Parts:");
    if (ImGui::BeginChild("Parts", {}, ImGuiChildFlags_FrameStyle)) {
        if (ImGui::BeginPopupContextWindow("Context menu")) {
            changed |= show_add_part_menu();
            ImGui::EndPopup();
        }

        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)) && m_selected_node != nullptr)
            remove_part(m_selected_node);

        for (const auto &part: m_robot->get_parts())
            changed |= show_part_list_item(part.get());
    }
    ImGui::EndChild();

    return changed;
}

bool RobotInspectorUI::show_part_list_item(Part *node) {
    ImGui::PushID(node);

    bool changed = false;

    std::string label = fmt::format("{} {}", node->get_icon(), node->get_name());
    const bool was_selected = ImGui::Selectable(label.c_str(), m_selected_node == node, ImGuiSelectableFlags_AllowOverlap);
    if (was_selected)
        m_selected_node = node;

    // Context menu (for removing or adding parts)
    if (ImGui::BeginPopupContextItem("Context menu")) {
        if (ImGui::MenuItem(ICON_FA_TRASH " Remove part")) {
            remove_part(node);
            changed = true;
        }

        changed |= show_add_part_menu();
        ImGui::EndPopup();
    }

    const float button_count = 3;
    const float button_size = ImGui::CalcTextSize(ICON_FA_ARROW_UP).x + ImGui::GetStyle().FramePadding.x * 2.0f;
    const float x_start = ImGui::GetContentRegionAvail().x - button_size * button_count - ImGui::GetStyle().ItemInnerSpacing.x * (button_count - 1) - ImGui::GetStyle().FramePadding.x;

    int part_index = m_robot->get_part_index(node);

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly)) {
        ImGui::SameLine(x_start);
        ImGui::BeginDisabled(part_index <= 0);
        if (ImGui::SmallButton(ICON_FA_ARROW_UP)) {
            move_part_up(node);
        }
        ImGui::SetItemTooltip("Move part up");
        ImGui::EndDisabled();

        ImGui::SameLine();
        ImGui::BeginDisabled(part_index >= m_robot->get_parts().size() - 1);
        if (ImGui::SmallButton(ICON_FA_ARROW_DOWN)) {
            move_part_down(node);
        }
        ImGui::SetItemTooltip("Move part down");
        ImGui::EndDisabled();

        ImGui::SameLine();
        if (ImGui::SmallButton(ICON_FA_TRASH)) {
            remove_part(node);
        }
        ImGui::SetItemTooltip("Remove part");
    }

    ImGui::PopID();
    return changed;
}

void RobotInspectorUI::show_unsaved_changes_modal() {
    if (ImGui::BeginPopupModal("Unsaved changes")) {
        ImGui::Text("The current robot has unsaved changes. If you continue, data may be lost. Are you sure?");
        if (ImGui::Button("Yes, discard changes")) {
            ImGui::CloseCurrentPopup();
            reload(/* force= */ true);
        }
        ImGui::SameLine();
        if (ImGui::Button("No, save changes")) {
            ImGui::CloseCurrentPopup();
            save();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

bool RobotInspectorUI::show_add_part_menu() {
    std::unique_ptr<Part> new_node;

#define MENU_ITEM(class_name, label)                                                      \
    if (ImGui::MenuItem(fmt::format("{} " label, class_name::get_static_icon()).c_str())) \
    new_node = std::make_unique<class_name>()

    if (ImGui::BeginMenu(ICON_FA_CIRCLE_PLUS " Add part...")) {
        MENU_ITEM(Led, "LED");

        if (ImGui::BeginMenu(ICON_FA_RULER " Sensors")) {
            MENU_ITEM(AngleSensor, "Angle sensor");
            MENU_ITEM(DistanceSensor, "Distance sensor");
            MENU_ITEM(ContactSensor, "Contact sensor");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(ICON_FA_LINK " Joint")) {
            MENU_ITEM(DistanceJoint, "Distance joint");
            MENU_ITEM(RevoluteJoint, "Revolute joint");
            MENU_ITEM(PrismaticJoint, "Prismatic joint");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(ICON_FA_SHAPES " Shape")) {
            MENU_ITEM(RectangleShape, "Rectangle");
            MENU_ITEM(CircleShape, "Circle");
            MENU_ITEM(PolygonShape, "Polygon");
            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }

#undef MENU_ITEM

    if (new_node != nullptr) {
        new_node->set_name(m_robot->get_new_part_name(new_node->get_kind_name()));

        if (m_selected_node != nullptr)
            insert_part(m_selected_node, new_node.release());
        else
            add_part(new_node.release());
        return true;
    }

    return false;
}

void RobotInspectorUI::move_part_up(Part *part) {
    assert(part != nullptr);

    const int index = (int) m_robot->get_part_index(part);
    if (index <= 0)
        return;// nothing to do

    m_delayed_operation.kind = DelayedOperation::Swap;
    m_delayed_operation.data.swap.from_index = index;
    m_delayed_operation.data.swap.to_index = index - 1;
}

void RobotInspectorUI::move_part_down(Part *part) {
    assert(part != nullptr);

    const int index = (int) m_robot->get_part_index(part);
    if (index >= m_robot->get_parts().size() - 1)
        return;// nothing to do

    m_delayed_operation.kind = DelayedOperation::Swap;
    m_delayed_operation.data.swap.from_index = index;
    m_delayed_operation.data.swap.to_index = index + 1;
}

void RobotInspectorUI::remove_part(Part *part) {
    assert(part != nullptr);

    const int index = (int) m_robot->get_part_index(part);
    m_delayed_operation.kind = DelayedOperation::Remove;
    m_delayed_operation.data.remove.part = part;
}

void RobotInspectorUI::insert_part(Part *after, Part *part) {
    assert(after != nullptr && part != nullptr);

    const int index = m_robot->get_part_index(after);
    m_delayed_operation.kind = DelayedOperation::Insert;
    m_delayed_operation.data.insert.index = index + 1;
    m_delayed_operation.data.insert.part = part;
}

void RobotInspectorUI::add_part(Part *part) {
    assert(part != nullptr);

    const int index = (int) m_robot->get_parts().size();
    m_delayed_operation.kind = DelayedOperation::Insert;
    m_delayed_operation.data.insert.index = index;
    m_delayed_operation.data.insert.part = part;
}

void RobotInspectorUI::execute_delayed_operation() {
    switch (m_delayed_operation.kind) {
        case DelayedOperation::None:// Nothing to do
            break;
        case DelayedOperation::Swap:
            m_robot->swap_parts(m_delayed_operation.data.swap.from_index, m_delayed_operation.data.swap.to_index);
            break;
        case DelayedOperation::Remove:
            if (m_selected_node == m_delayed_operation.data.remove.part)
                m_selected_node = nullptr;
            m_robot->remove_part(m_delayed_operation.data.remove.part);
            break;
        case DelayedOperation::Insert:
            m_robot->insert_part(m_delayed_operation.data.insert.index, m_delayed_operation.data.insert.part);
            break;
        default:
            assert(false && "unknown delayed operation kind");
    }

    m_delayed_operation.kind = DelayedOperation::None;
}

#include "physics_robot.hpp"

void RobotInspectorUI::check_constraints() {
    SPDLOG_TRACE("Checking robot constraints...");

    b2World world{b2Vec2_zero};

    b2BodyDef ground_body_def;
    b2Body *ground_body = world.CreateBody(&ground_body_def);

    PhysicsRobot robot(m_robot, &world, ground_body);

    // We use Box2D to check for constraints between parts.
    // All objects are static. We only check for collisions and joints constraints.
    int num_steps = 5;
    while (num_steps-- > 0) {
        world.Step(1.0f / 60.0f, 6, 2);
    }

    robot.sync_from_box2d();
}
