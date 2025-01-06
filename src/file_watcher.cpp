#include "file_watcher.hpp"
#include <spdlog/spdlog.h>

FileWatcher::FileWatcher(const std::filesystem::path &path) {
    m_path = path;
    m_last_write_time = std::filesystem::last_write_time(m_path);
}

bool FileWatcher::check() {
    std::filesystem::file_time_type current_write_time = std::filesystem::last_write_time(m_path);
    if (current_write_time != m_last_write_time) {
        m_callback(false);
        m_last_write_time = current_write_time;
        return true;
    }

    return false;
}
