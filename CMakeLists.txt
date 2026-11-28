cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(LREngineWorkspace
    VERSION 0.1
    LANGUAGES CXX
)

# Collect source files
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/src/*.cpp"
)

file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Editor/src/*.cpp"
)

list(REMOVE_ITEM EDITOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Editor/src/main.cpp"
)

# Collect headers
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/include/*.hpp"
)

file(GLOB_RECURSE EDITOR_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Editor/include/*.hpp"
)

# Create library
add_library(LRLibrary SHARED
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
    ${EDITOR_SOURCES}
    ${EDITOR_HEADERS}
)

# Include directories
target_include_directories(LRLibrary PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Engine/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/Editor/include"
)

set_target_properties(LRLibrary PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

add_subdirectory(Engine)
add_subdirectory(Editor)

enable_testing()
add_subdirectory(Tests)
