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

void RobotInspectorUI::show(bool &should_show_robot_inspector, bool &should_show_part_inspector, bool &should_show_preview) {
    bool changed = false;

    if (should_show_preview)
        changed |= show_preview(should_show_preview);

    if (should_show_robot_inspector)
        changed |= show_robot_inspector(should_show_robot_inspector);

    if (should_show_part_inspector)
        changed |= show_part_inspector(should_show_part_inspector);

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

bool RobotInspectorUI::show_preview(bool &should_show) {
    if (ImGui::Begin(ICON_FA_ROBOT " Robot Preview", &should_show)) {
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

bool RobotInspectorUI::show_robot_inspector(bool &should_show) {
    if (m_robot == nullptr) {
        ImGui::Begin(ICON_FA_ROBOT " Robot inspector", &should_show);
        ImGui::Text("No robot loaded...");
        ImGui::End();
        return false;
    }

    bool changed = false;

    if (ImGui::Begin(ICON_FA_ROBOT " Robot inspector", &should_show)) {
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
        else
            ImGui::Text("No changes to save");

        changed |= show_part_list();

        show_unsaved_changes_modal();
    }
    ImGui::End();

    return changed;
}

bool RobotInspectorUI::show_part_inspector(bool &should_show) {
    bool changed = false;

    if (ImGui::Begin(ICON_FA_PUZZLE_PIECE " Part inspector", &should_show)) {
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
    if (ImGui::BeginChild("Parts", {})) {
        if (ImGui::BeginPopupContextWindow("Context menu")) {
            changed |= show_add_part_menu();
            ImGui::EndPopup();
        }

        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)) && m_selected_node != nullptr)
            remove_part(m_selected_node);

        for (const auto &part: m_robot->get_parts()) {
            // Only show top-level parts
            if (part->get_parent_part() == nullptr)
                changed |= show_part_list_item(part.get());
        }
    }
    ImGui::EndChild();

    return changed;
}

bool RobotInspectorUI::show_part_list_item(Part *node) {
    ImGui::PushID(node);

    bool changed = false;

    const bool is_folder = node->is<FolderPart>();

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_DefaultOpen;
    if (m_selected_node == node)
        flags |= ImGuiTreeNodeFlags_Selected;
    if (!is_folder)
        flags |= ImGuiTreeNodeFlags_Leaf;

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { -ImGui::GetFontSize() / 4.0f, ImGui::GetStyle().FramePadding.y });
    const bool is_open = ImGui::TreeNodeEx(node->get_name().c_str(), flags, "");
    ImGui::PopStyleVar();

    // Context menu (for removing or adding parts)
    if (ImGui::BeginPopupContextItem("Context menu")) {
        m_selected_node = node;

        changed |= show_add_part_menu(node);

        if (ImGui::MenuItem(ICON_FA_TRASH " Remove part")) {
            remove_part(node);
            changed = true;
        }

        if (ImGui::MenuItem(ICON_FA_PEN " Rename part")) {
            rename_part(node);
        }

        if (ImGui::MenuItem(ICON_FA_ARROW_UP " Move up", nullptr, nullptr, can_move_part_up(node))) {
            move_part_up(node);
            changed = true;
        }

        if (ImGui::MenuItem(ICON_FA_ARROW_DOWN " Move down", nullptr, nullptr, can_move_part_down(node))) {
            move_part_down(node);
            changed = true;
        }

        ImGui::EndPopup();
    }

    const bool is_hovered = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly);
    if (ImGui::IsItemClicked(0))
        m_selected_node = node;

    if (ImGui::BeginDragDropSource()) {
        ImGui::SetDragDropPayload("PART", &node, sizeof(Part*));
        ImGui::Text("%s %s", node->get_icon(), node->get_name().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PART")) {
            Part* payload_node = *(Part**)payload->Data;
            if (payload_node != node) {
                m_delayed_operation.kind = is_folder ? DelayedOperation::MoveInto : DelayedOperation::MoveAfter;
                m_delayed_operation.data.move.part = payload_node;
                m_delayed_operation.data.move.target = node;
            }
        }
        ImGui::EndDragDropTarget();
    }

    // Show text, there is a special code to be able to use a custom icon color for folders.
    ImGui::SameLine();
    const char* icon = is_folder ? (is_open ? ICON_FA_FOLDER_OPEN : ICON_FA_FOLDER) : node->get_icon();

    ImVec4 icon_color;
    if (is_folder) {
        const auto* folder = node->as<FolderPart>();
        const auto folder_color = folder->get_color();
        icon_color = { folder_color.x, folder_color.y, folder_color.z, 1.0f };
    } else if (auto* shape = node->as<Shape>(); shape != nullptr) {
        const auto shape_color = shape->get_color();
        icon_color = { shape_color.x, shape_color.y, shape_color.z, 1.0f };
    } else {
        // Default icon color
        icon_color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
    }

    ImGui::TextColored(icon_color, icon);
    ImGui::SameLine(0.f, 0.f);

    if (m_renamed_node == node) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });

        std::string name = m_renamed_node->get_name();
        if (ImGui::InputText("##rename", &name, ImGuiInputTextFlags_AutoSelectAll |ImGuiInputTextFlags_EnterReturnsTrue)) {
            if (name != m_renamed_node->get_name()) {
                m_renamed_node->set_name(name);
                changed = true;
            }

            m_renamed_node = nullptr;
        }
        ImGui::PopStyleVar();

        if (!ImGui::IsItemFocused() && !m_first_time_renaming)
            m_renamed_node = nullptr;

        if (m_first_time_renaming) {
            ImGui::SetKeyboardFocusHere(-1);
            m_first_time_renaming = false;
        }
    } else {
        ImGui::Text(" %s", node->get_name().c_str());
    }

    // Buttons for moving parts on right side
    if (is_hovered && m_renamed_node != node) {
        const float button_count = 3;
        const float button_height = ImGui::CalcTextSize(ICON_FA_ARROW_UP).y;
        const float button_width = ImGui::CalcTextSize(ICON_FA_ARROW_UP).x + ImGui::GetStyle().FramePadding.x;
        const float total_buttons_width = button_width * button_count + (ImGui::GetStyle().ItemSpacing.x / 2.f) * (button_count - 1);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { ImGui::GetStyle().ItemSpacing.x / 2.f, ImGui::GetStyle().ItemSpacing.y});
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { ImGui::GetStyle().FramePadding.x / 2.f, 0.0f });

        // Move up button
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - total_buttons_width);
        ImGui::BeginDisabled(!can_move_part_up(node));
        if (ImGui::Button(ICON_FA_ARROW_UP, { button_width, button_height }))
            move_part_up(node);
        ImGui::SetItemTooltip("Move part up");
        ImGui::EndDisabled();

        // Move down button
        ImGui::SameLine();
        ImGui::BeginDisabled(!can_move_part_down(node));
        if (ImGui::Button(ICON_FA_ARROW_DOWN, { button_width, button_height }))
            move_part_down(node);
        ImGui::SetItemTooltip("Move part down");
        ImGui::EndDisabled();

        // Remove button
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TRASH, { button_width, button_height }))
            remove_part(node);
        ImGui::SetItemTooltip("Remove part");

        ImGui::PopStyleVar(2);
    }

    // Recursively show children for folders
    if (is_open) {
        if (is_folder) {
            auto* folder = node->as<FolderPart>();
            for (const auto &part: folder->get_parts())
                changed |= show_part_list_item(part);
        }

        ImGui::TreePop();
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

bool RobotInspectorUI::show_add_part_menu(Part* parent_node) {
    std::unique_ptr<Part> new_node;

#define MENU_ITEM(class_name, label)                                                      \
    if (ImGui::MenuItem(fmt::format("{} " label, class_name::get_static_icon()).c_str())) \
    new_node = std::make_unique<class_name>()

    if (ImGui::BeginMenu(ICON_FA_CIRCLE_PLUS " Add part...")) {
        MENU_ITEM(FolderPart, "Folder");

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
        new_node->set_name(m_robot->get_unique_part_name(new_node->get_kind_name()));

        if (parent_node != nullptr && !parent_node->is<FolderPart>())
            insert_part(parent_node, new_node.release());
        else
            add_part(new_node.release(), parent_node);
        return true;
    }

    return false;
}

bool RobotInspectorUI::can_move_part_up(Part *part) const {
    assert(part != nullptr);

    const auto* folder_part = part->get_folder_parent();
    if (folder_part != nullptr)
        return true; // either move up or get out of folder

    int index = m_robot->get_part_index(part);
    return index > 0;
}

bool RobotInspectorUI::can_move_part_down(Part *part) const {
    assert(part != nullptr);

    const auto* folder_part = part->get_folder_parent();
    if (folder_part != nullptr)
        return true; // either move down or get out of folder

    int index = m_robot->get_part_index(part);
    return index < m_robot->get_parts().size() - 1;
}

void RobotInspectorUI::move_part_up(Part *part) {
    assert(part != nullptr);

    const int index = (int) m_robot->get_part_index(part);
    if (index <= 0)
        return;// nothing to do

    // FIXME: support folders
    m_delayed_operation.kind = DelayedOperation::Swap;
    m_delayed_operation.data.swap.from_index = index;
    m_delayed_operation.data.swap.to_index = index - 1;
}

void RobotInspectorUI::move_part_down(Part *part) {
    assert(part != nullptr);

    const int index = (int) m_robot->get_part_index(part);
    if (index >= m_robot->get_parts().size() - 1)
        return;// nothing to do

    // FIXME: support folders
    m_delayed_operation.kind = DelayedOperation::Swap;
    m_delayed_operation.data.swap.from_index = index;
    m_delayed_operation.data.swap.to_index = index + 1;
}

void RobotInspectorUI::rename_part(Part *part) {
    m_renamed_node = part;
    m_first_time_renaming = true;
}

void RobotInspectorUI::remove_part(Part *part) {
    assert(part != nullptr);

    m_delayed_operation.kind = DelayedOperation::Remove;
    m_delayed_operation.data.remove.part = part;
}

void RobotInspectorUI::insert_part(Part *after, Part *part) {
    assert(after != nullptr && part != nullptr);

    m_delayed_operation.kind = DelayedOperation::Insert;
    m_delayed_operation.data.insert.after = after;
    m_delayed_operation.data.insert.part = part;
}

void RobotInspectorUI::add_part(Part *part, Part* parent_part) {
    assert(part != nullptr);

    m_delayed_operation.kind = DelayedOperation::Add;
    m_delayed_operation.data.add.parent = parent_part;
    m_delayed_operation.data.add.part = part;
}

void RobotInspectorUI::execute_delayed_operation() {
    switch (m_delayed_operation.kind) {
        case DelayedOperation::None:// Nothing to do
            break;
        case DelayedOperation::Swap:
            m_robot->swap_parts(m_delayed_operation.data.swap.from_index, m_delayed_operation.data.swap.to_index);
            m_dirty = true;
            break;
        case DelayedOperation::Remove: {
            assert(m_delayed_operation.data.remove.part != nullptr);

            if (m_selected_node == m_delayed_operation.data.remove.part)
                m_selected_node = nullptr;

            auto *parent_folder = m_delayed_operation.data.remove.part->get_folder_parent();
            if (parent_folder != nullptr)
                parent_folder->remove_part(m_delayed_operation.data.remove.part);
            else
                m_robot->remove_part(m_delayed_operation.data.remove.part);

            m_dirty = true;
        } break;
        case DelayedOperation::Insert: {
            assert(m_delayed_operation.data.insert.part != nullptr);
            assert(m_delayed_operation.data.insert.after != nullptr);

            auto* parent_folder= m_delayed_operation.data.insert.after->get_folder_parent();
            if (parent_folder != nullptr) {
                const int index = parent_folder->get_part_index(m_delayed_operation.data.insert.after);
                parent_folder->insert_part(index + 1, m_delayed_operation.data.insert.part);
            } else {
                const int index = m_robot->get_part_index(m_delayed_operation.data.insert.after);
                m_robot->insert_part(index + 1, m_delayed_operation.data.insert.part);
            }

            m_dirty = true;
        } break;
        case DelayedOperation::Add: {
            assert(m_delayed_operation.data.add.part != nullptr);

            if (m_delayed_operation.data.add.parent != nullptr) {
                auto* folder = m_delayed_operation.data.add.parent->as<FolderPart>();
                assert(folder != nullptr);
                m_robot->add_part(m_delayed_operation.data.add.part);
                folder->add_part(m_delayed_operation.data.add.part);
            } else {
                m_robot->add_part(m_delayed_operation.data.add.part);
            }

            m_dirty = true;
        } break;
        case DelayedOperation::MoveAfter: {
            assert(m_delayed_operation.data.move.part != nullptr);
            assert(m_delayed_operation.data.move.target != nullptr);

            auto* part = m_delayed_operation.data.move.part;
            auto* target = m_delayed_operation.data.move.target;
            if (part == target)
                break;

            // Remove part from its old location
            m_robot->release_part(part);

            // Reinsert it after target
            auto* target_folder_parent = target->get_folder_parent();
            if (target_folder_parent != nullptr) {
                const int index = target_folder_parent->get_part_index(target);
                m_robot->add_part(part);
                target_folder_parent->insert_part(index + 1, part);
            } else {
                const int index = m_robot->get_part_index(target);
                m_robot->insert_part(index + 1, part);
            }

            m_dirty = true;
        } break;
        case DelayedOperation::MoveInto: {
            assert(m_delayed_operation.data.move.part != nullptr);
            assert(m_delayed_operation.data.move.target != nullptr);

            auto* part = m_delayed_operation.data.move.part;
            auto* target = m_delayed_operation.data.move.target;
            if (part == target)
                break;

            auto* folder = target->as<FolderPart>();
            assert(folder != nullptr);

            auto* parent_folder = part->get_folder_parent();
            if (parent_folder != nullptr)
                parent_folder->remove_part(part);

            folder->add_part(part);
            m_dirty = true;
        } break;
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
