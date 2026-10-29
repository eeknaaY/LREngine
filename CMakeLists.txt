cmake_minimum_required(VERSION 3.20.0)

project(LREngine
    VERSION 0.1
    DESCRIPTION "Vulkan Engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)
include(FetchContent)

file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(LREngine
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(LREngine PRIVATE
    "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(LREngine PRIVATE Vulkan::Vulkan)

