#include "shared_library.hpp"

#include <spdlog/spdlog.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

SharedLibrary::SharedLibrary(const std::string &library_path)
    : m_path(library_path) {
}

SharedLibrary::~SharedLibrary() {
    unload();
}

bool SharedLibrary::load() {
    if (m_handle != nullptr)
        return true;

#ifdef _WIN32
    m_handle = LoadLibraryA(m_path.c_str());
#else
    m_handle = dlopen(m_path.c_str(), RTLD_LAZY);
#endif

    if (m_handle == nullptr)
        return false;
    return true;
}

void SharedLibrary::unload() {
    if (m_handle == nullptr)
        return;

#ifdef _WIN32
    FreeLibrary(static_cast<HMODULE>(m_handle));
#else
    dlclose(m_handle);
#endif
    m_handle = nullptr;
}

void *SharedLibrary::get(const char *symbol_name) {
    if (m_handle == nullptr)
        return nullptr;

#ifdef _WIN32
    return (void *) GetProcAddress(static_cast<HMODULE>(m_handle), symbol_name);
#else
    return dlsym(m_handle, symbol_name);
#endif
}
