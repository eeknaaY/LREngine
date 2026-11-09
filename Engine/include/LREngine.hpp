#ifndef LRENGINE_HPP
#define LRENGINE_HPP

#include <Export.hpp>

#include "LRWindow.hpp"
#include "Graphics/LRRenderer.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

typedef class LREngine {
public:
	LREngine(LR::API_TYPE Type);
	~LREngine();

	void Update();
	bool IsActive() const { return m_IsActive; }

	LRWindow* CreateWindow(const glm::vec2& size, SDL_WindowFlags flags, const char* title);

private:
	bool m_IsActive = true;
	LRWindow* m_Window = nullptr;
	const LR::API_TYPE m_API_Type;

	vk::raii::Context m_VkContext;
	vk::raii::Instance m_VkInstance{ nullptr };

	vk::ApplicationInfo m_VkApplicationInfo;
	vk::InstanceCreateInfo m_VkInstanceCreateInfo;
	vk::DeviceCreateInfo m_VkDeviceCreateInfo;

	vk::raii::PhysicalDevice m_VkPhysicalDevice{ nullptr };
	vk::raii::Device m_VkDevice{ nullptr };

	vk::raii::SwapchainKHR* m_SwapChain = nullptr;
	std::vector<vk::Image> m_SwapChainImages;

} LRE;



#endif