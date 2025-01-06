#pragma once

#include "draw/renderer_2d.hpp"
#include "robot.hpp"

class RobotInspectorUI {
public:
    RobotInspectorUI();
    explicit RobotInspectorUI(const std::shared_ptr<Robot> &robot);

    [[nodiscard]] const std::shared_ptr<Robot> &get_robot() const { return m_robot; }
    void set_robot(const std::shared_ptr<Robot> &robot);

    void show();

    // Dirty flag.
    [[nodiscard]] bool is_dirty() const { return m_dirty; }
    void set_dirty(bool dirty) { m_dirty = dirty; }

    [[nodiscard]] Part *get_selected_node() const { return m_selected_node; }


    void reload(bool force = false);
    void save();

private:
    bool show_preview();
    bool show_robot_inspector();
    bool show_part_inspector();
    bool show_part_list();
    bool show_part_list_item(Part *node);
    bool show_add_part_menu();

    void show_unsaved_changes_modal();

    void check_constraints();

    struct DelayedOperation {
        enum Kind {
            None,
            Swap,
            Remove,
            Insert
        };

        Kind kind = None;
        union {
            struct {
                int from_index;
                int to_index;
            } swap;
            struct {
                Part *part;
            } remove;
            struct {
                int index;
                Part *part;
            } insert;
        } data;
    };// struct DelayedOperation

    DelayedOperation m_delayed_operation;

    void move_part_up(Part *part);
    void move_part_down(Part *part);
    void remove_part(Part *part);
    void insert_part(Part *after, Part *part);
    void add_part(Part *part);
    void execute_delayed_operation();

private:
    std::shared_ptr<Robot> m_robot;
    Part *m_selected_node = nullptr;

    Renderer2D m_renderer;
    SceneView m_scene_view{m_renderer};
    bool m_show_preview = true;
    bool m_show_part_inspector = true;

    bool m_check_constraints = true;
    bool m_dirty = false;
};// class RobotInspectorUI
