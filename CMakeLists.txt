cmake_minimum_required(VERSION 3.28)
project(robots)

set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)

add_executable(robots src/main.cpp
        src/robot/robot_parser.cpp
        src/robot/robot_parser.cpp
        src/robot/robot_parser.hpp
        src/parts/part.hpp
        src/robot/robot.cpp
        src/parts/part.cpp
        src/parts/sensor.hpp
        src/parts/sensor.cpp
        src/robot/robot_inspector_ui.hpp
        src/robot/robot_inspector_ui.cpp
        src/parts/shape.hpp
        src/parts/shape.cpp
        src/base_application.hpp
        src/base_application.cpp
        src/simulation/simulation.cpp
        src/simulation/simulation.hpp
        src/simulation/simulation.cpp
        src/simulation/simulation.hpp
        src/draw/shader.cpp
        src/draw/shader.hpp
        src/draw/renderer_2d.hpp
        src/draw/renderer_2d.cpp
        src/simulation/simulation_view.hpp
        src/simulation/simulation_view.cpp
        src/robot/robot_saver.hpp
        src/robot/robot_saver.cpp
        src/parts/joint.hpp
        src/parts/joint.cpp
        src/parts/part_manager.hpp
        src/parts/part_manager.cpp
        src/robot/physics_robot.hpp
        src/robot/physics_robot.cpp
        src/robot/robot_ai.hpp
        src/robot/robot_ai.cpp
        src/robot/lua_robot_ai.hpp
        src/robot/lua_robot_ai.cpp
        src/robot/lua_robot_ai.hpp
        src/application.hpp
        src/application.cpp
        src/file_watcher.hpp
        src/file_watcher.cpp
)
target_include_directories(robots PRIVATE src)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(robots PRIVATE spdlog::spdlog)

find_package(ImGui CONFIG REQUIRED)
target_link_libraries(robots PRIVATE imgui::imgui)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(robots PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(robots PRIVATE glad::glad)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(robots PRIVATE box2d::box2d)

find_package(yaml-cpp CONFIG REQUIRED)
target_link_libraries(robots PRIVATE yaml-cpp::yaml-cpp)

find_package(glm CONFIG REQUIRED)
target_link_libraries(robots PRIVATE glm::glm)

find_package(Lua REQUIRED)
target_link_libraries(robots PRIVATE ${LUA_LIBRARIES})

find_package(tinyfiledialogs CONFIG REQUIRED)
target_link_libraries(robots PRIVATE tinyfiledialogs::tinyfiledialogs)
