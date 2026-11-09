#include "LRWindow.hpp"
#include <stdexcept>

LRWindow::LRWindow(const glm::vec2& size, SDL_WindowFlags flags, const char* title, const vk::raii::Instance& instance) {
    SDL_Window* window = SDL_CreateWindow(
        title,
        size.x,
        size.y,
        flags
    );

    if (!window) throw std::runtime_error(SDL_GetError());

    VkSurfaceKHR rawSurface = VK_NULL_HANDLE;

    if (!SDL_Vulkan_CreateSurface(
        window,
        static_cast<VkInstance>(*instance),
        nullptr,
        &rawSurface
    ))
    {
        throw std::runtime_error(SDL_GetError());
    }

    m_Window = vk::SurfaceKHR{ rawSurface };
    Size = size;
}

LRWindow::~LRWindow() {

}

bool LRWindow::PollEvents(SDL_Event* event) const {
    return SDL_PollEvent(event);
}
