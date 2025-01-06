#pragma once

#include <filesystem>
#include <functional>

// A simple file watcher that checks if a file has been modified.
class FileWatcher {
public:
    explicit FileWatcher(const std::filesystem::path &path);

    // Set the callback function to be called when the file is modified.
    // The function takes a boolean parameter that is true if the file has been deleted.
    void set_callback(std::function<void(bool)> callback) { m_callback = std::move(callback); }

    // Get the path of the file being watched.
    [[nodiscard]] const auto& get_path() const { return m_path; }

    // Check if the file has been modified since the last check.
    // Returns true if the file has been modified, false otherwise.
    bool check();

private:
    std::filesystem::path m_path;
    std::filesystem::file_time_type m_last_write_time;
    std::function<void(bool)> m_callback;
};// class FileWatcher
