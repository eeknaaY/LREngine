
cmake_minimum_required(VERSION 3.20)

project(LREngine
    VERSION 0.1
    DESCRIPTION "Vulkan Engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Don't build dependency examples or tests.
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)

set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE BOOL "" FORCE)

set(FASTGLTF_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(FASTGLTF_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FASTGLTF_ENABLE_DOCS OFF CACHE BOOL "" FORCE)

# --------------------------------------------------
# SDL3
# --------------------------------------------------

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

# --------------------------------------------------
# Vulkan SDK
# --------------------------------------------------

find_package(Vulkan REQUIRED)

# --------------------------------------------------
# Vulkan Memory Allocator (VMA)
# --------------------------------------------------

FetchContent_Declare(
    VulkanMemoryAllocator
    GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
    GIT_TAG v3.2.1
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(VulkanMemoryAllocator)

# --------------------------------------------------
# GLM
# --------------------------------------------------

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glm)

# --------------------------------------------------
# stb_image
# --------------------------------------------------

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(stb)

add_library(stb_image INTERFACE)

target_include_directories(stb_image INTERFACE
    "${stb_SOURCE_DIR}"
)

# --------------------------------------------------
# Dear ImGui
# --------------------------------------------------

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.8
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"

    # SDL3 platform backend
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"

    # Vulkan renderer backend
    "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp"
)

target_include_directories(imgui PUBLIC
    "${imgui_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}/backends"
)

target_link_libraries(imgui PUBLIC
    SDL3::SDL3
    Vulkan::Vulkan
)

# --------------------------------------------------
# fastgltf
# --------------------------------------------------

FetchContent_Declare(
    fastgltf
    GIT_REPOSITORY https://github.com/spnda/fastgltf.git
    GIT_TAG v0.8.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(fastgltf)


file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(LREngine SHARED
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(LREngine
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(LREngine PUBLIC
    SDL3::SDL3
    Vulkan::Vulkan
    GPUOpen::VulkanMemoryAllocator
    glm::glm
    stb_image
    imgui
    fastgltf
)

target_compile_definitions(LREngine
    PRIVATE LRENGINE_EXPORTS
)

set_target_properties(LREngine PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

# --------------------------------------------------
# Compile Shaders
# --------------------------------------------------


find_program(
    SLANGC_EXECUTABLE
    NAMES slangc slangc.exe
    HINTS "$ENV{VULKAN_SDK}/Bin"
    REQUIRED
)

function(add_slang_shader_target TARGET)

    cmake_parse_arguments(
        PARSE_ARGV 1 SHADER
        ""
        ""
        "SOURCES"
    )

    if(NOT SHADER_SOURCES)
        message(FATAL_ERROR "No Slang shader sources provided!")
    endif()

    set(SHADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

    set(SHADER_OUTPUTS "")

    foreach(SOURCE IN LISTS SHADER_SOURCES)

        get_filename_component(
            SHADER_NAME
            "${SOURCE}"
            NAME_WE
        )

        set(SHADER_OUTPUT "${SHADERS_DIR}/${SHADER_NAME}.spv")

        add_custom_command(
            OUTPUT "${SHADER_OUTPUT}"

            COMMAND "${CMAKE_COMMAND}" -E make_directory "${SHADERS_DIR}"

            COMMAND "${SLANGC_EXECUTABLE}"
                "${SOURCE}"
                -target spirv
                -profile spirv_1_4
                -emit-spirv-directly
                -fvk-use-entrypoint-name
                -o "${SHADER_OUTPUT}"

            DEPENDS "${SOURCE}"

            COMMENT "Compiling ${SHADER_NAME}.slang"
            VERBATIM
        )

        list(APPEND SHADER_OUTPUTS "${SHADER_OUTPUT}")

    endforeach()

    add_custom_target(
        ${TARGET}
        DEPENDS ${SHADER_OUTPUTS}
    )

endfunction()



file(GLOB SHADER_SLANG_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.slang"
)

set(SHADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")
set(SHADER_OUTPUT "${SHADERS_DIR}/slang.spv")

add_slang_shader_target(
    shaders
    SOURCES ${SHADER_SLANG_SOURCES}
)

add_dependencies(LREngine shaders)