cmake_minimum_required(VERSION 3.20.0)

project(LREngine_Editor
    VERSION 0.1
    DESCRIPTION "Vulkan Engine Editor"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(LREngine_Editor
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(LREngine_Editor PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Link engine to game
target_link_libraries(LREngine_Editor PRIVATE LREngine)

add_custom_command(TARGET LREngine_Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:LREngine>"
        "$<TARGET_FILE_DIR:LREngine_Editor>"
)

add_custom_command(TARGET LREngine_Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL3::SDL3>"
        "$<TARGET_FILE_DIR:LREngine_Editor>"
)