#ifndef LRWINDOW_HPP
#define LRWINDOW_HPP

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <glm/vec2.hpp>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace LR {

class Window {
public:
	Window(const glm::vec2& size, SDL_WindowFlags flags, const char* title);
	~Window();

	glm::vec2 Size;

	operator vk::SurfaceKHR() const { return m_Surface; }

	bool PollEvents(SDL_Event* event) const;
	void CreateSurface(const vk::raii::Instance& instance);


private:
	SDL_Window* m_Window = nullptr;
	vk::raii::SurfaceKHR m_Surface{ nullptr };
};

}

#endif