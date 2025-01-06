#include "part_manager.hpp"

PartManager &PartManager::get() {
    static PartManager instance;
    return instance;
}

void PartManager::register_part(std::string_view kind_name, PartMetaInfo *meta_info) {
    get().m_registered_parts[kind_name] = meta_info;
}

void PartManager::unregister_part(std::string_view kind_name) {
    get().m_registered_parts.erase(kind_name);
}

PartMetaInfo *PartManager::get_part_meta_info(std::string_view kind_name) {
    auto &registered_parts = get().m_registered_parts;
    const auto it = registered_parts.find(kind_name);
    if (it == registered_parts.end())
        return nullptr;
    return it->second;
}

std::string PartManager::compute_display_name(const char *class_name) {
    std::string display_name;

    for (const char *it = class_name; *it != '\0'; ++it) {
        if (it == class_name) {// First character
            display_name += (char)std::toupper(*it);
        } else if (std::isupper(*it)) {
            display_name += ' ';
            display_name += (char)std::tolower(*it);
        } else {
            display_name += *it;
        }
    }

    return display_name;
}
