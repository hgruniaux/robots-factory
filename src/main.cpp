#include "application.hpp"
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);

    Application app;
    if (!app.init()) {
        SPDLOG_CRITICAL("Failed to initialize simulator");
        return EXIT_FAILURE;
    }

    SPDLOG_INFO("Simulator initialized successfully");

    app.run();

    app.shutdown();
    return EXIT_SUCCESS;
}
