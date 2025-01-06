#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>

class Part;

struct PartMetaInfo {
    // The name of the part class. Used in serialization and deserialization.
    // Example: "LED", "DistanceJoint", "FloatSensor", etc.
    const char *class_name;
    // The icon of the part class. Must be a FontAwesome icon name.
    // Example: ICON_FA_LIGHTBULB, ICON_FA_RULER, ICON_FA_COMPASS, etc.
    const char *icon = nullptr;
    // The display name of the part class. Used in the GUI.
    // Example: "LED", "Distance joint", "Float sensor", etc.
    std::string display_name;
    // The function to create a part instance of this class.
    std::function<std::unique_ptr<Part>()> create_function;
};// struct PartMetaInfo

class PartManager {
public:
    // Get the singleton instance of the part manager.
    static PartManager &get();

    template<class T>
    static PartMetaInfo register_part(const char *class_name, const char *icon) {
        static PartMetaInfo meta_info;
        meta_info.class_name = class_name;
        meta_info.icon = icon;
        meta_info.display_name = compute_display_name(class_name);
        meta_info.create_function = []() { return std::make_unique<T>(); };
        register_part(meta_info.class_name, &meta_info);
        return meta_info;
    }

    static void register_part(std::string_view kind_name, PartMetaInfo *meta_info);
    static void unregister_part(std::string_view kind_name);

    [[nodiscard]] static PartMetaInfo *get_part_meta_info(std::string_view kind_name);

private:
    [[nodiscard]] static std::string compute_display_name(const char* class_name);

private:
    std::unordered_map<std::string_view, PartMetaInfo *> m_registered_parts;
};// class PartManager
