#ifndef LRVULKANAPI_HPP
#define LRVULKANAPI_HPP

#include "GraphicsAPI.hpp"
#include "../Window.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace LR {
class VulkanAPI : public GraphicsAPI {
public:
	void Draw() override;
	bool Initialize() override;

	void CreateSwapChain(Window* m_Window);
	const vk::raii::Instance& GetInstance() { return m_VkInstance; }

private:
	vk::raii::Context m_VkContext;
	vk::raii::Instance m_VkInstance{ nullptr };

	vk::ApplicationInfo m_VkApplicationInfo;
	vk::InstanceCreateInfo m_VkInstanceCreateInfo;
	vk::DeviceCreateInfo m_VkDeviceCreateInfo;

	vk::raii::PhysicalDevice m_VkPhysicalDevice{ nullptr };
	vk::raii::Device m_VkDevice{ nullptr };

	vk::raii::SwapchainKHR m_SwapChain{ nullptr };
	std::vector<vk::Image> m_SwapChainImages;
};
}

#endif