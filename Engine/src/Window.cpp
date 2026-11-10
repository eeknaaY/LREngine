#include "LR/Window.hpp"
#include <stdexcept>

namespace LR {

Window::Window(const glm::vec2& size, SDL_WindowFlags flags, const char* title) {
    SDL_Window* window = SDL_CreateWindow(
        title,
        size.x,
        size.y,
        flags
    );

    if (!window) throw std::runtime_error(SDL_GetError());

    m_Window = window;
    Size = size;
}

Window::~Window() {

}

bool Window::PollEvents(SDL_Event* event) const {
    return SDL_PollEvent(event);
}

void Window::CreateSurface(const vk::raii::Instance& instance) {
    VkSurfaceKHR rawSurface = VK_NULL_HANDLE;

    if (!SDL_Vulkan_CreateSurface(
        m_Window,
        static_cast<VkInstance>(*instance),
        nullptr,
        &rawSurface
    ))
    {
        throw std::runtime_error(SDL_GetError());
    }

    m_Surface = vk::raii::SurfaceKHR(
        instance,
        vk::SurfaceKHR{ rawSurface }
    );
}

}
