add_library(lustre_lib STATIC
        lib/lib.cpp
        lib/lib.h
        lib/lib_types.h)

function(add_lustre_ai name)
    add_library(${name} SHARED
            ${name}/robot.cpp
            ${name}/robot_c/robot.c
            ${name}/robot_c/robot.h
            ${name}/robot_c/robot_types.c
            ${name}/robot_c/robot_types.h)
    target_link_libraries(${name} PRIVATE lustre_lib robots_core)
    target_include_directories(${name} PRIVATE lib)
    target_include_directories(${name} PRIVATE ${name}_c)
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
    set_target_properties(${name} PROPERTIES PREFIX "robotai_")
endfunction()

add_lustre_ai(spider)
add_lustre_ai(spider2)
