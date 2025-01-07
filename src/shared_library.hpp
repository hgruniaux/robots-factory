#pragma once

#include <string>

class SharedLibrary {
public:
    explicit SharedLibrary(const std::string &library_path);
    ~SharedLibrary();

    SharedLibrary(const SharedLibrary &) = delete;
    SharedLibrary &operator=(const SharedLibrary &) = delete;

    [[nodiscard]] const std::string& get_path() const { return m_path; }

    [[nodiscard]] bool is_valid() const { return m_handle != nullptr; }
    bool load();
    void unload();

    [[nodiscard]] void *get(const char *symbol_name);
    template<class T>
    [[nodiscard]] T get(const char *symbol_name) { return (T) (get(symbol_name)); }

private:
    std::string m_path;
    void *m_handle = nullptr;
};// class SharedLibrary
