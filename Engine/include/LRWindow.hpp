#ifndef LRWINDOW_HPP
#define LRWINDOW_HPP

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
#include <vulkan/vulkan.hpp>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan_raii.hpp>

class LRWindow {
public:
	LRWindow(const glm::vec2& size, SDL_WindowFlags flags, const char* title, const vk::raii::Instance& instance);
	~LRWindow();

public:
	glm::vec2 Size;

public:
	bool PollEvents(SDL_Event* event) const;

	operator vk::SurfaceKHR() const { return m_Window; }

private:
	vk::SurfaceKHR m_Window = nullptr;
};

#endif