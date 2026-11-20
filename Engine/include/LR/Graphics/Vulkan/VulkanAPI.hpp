#ifndef LRVULKANAPI_HPP
#define LRVULKANAPI_HPP

#include "../Interfaces/GraphicsAPI.hpp"
#include "../../Window.hpp"
#include "../../Scene/SubScene.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace LR {
class VulkanAPI : public GraphicsAPI {
public:
	VulkanAPI();
	~VulkanAPI();

	void Draw(const SubScene& subscene) override;

	const vk::raii::Device& GetDevice() const { return m_VkDevice; }
	const vk::SurfaceFormatKHR& GetSurfaceFormat() const { return m_VkSurfaceFormatKHR; }
	const vk::raii::Instance& GetInstance() const { return m_VkInstance; }
	const vk::Extent2D& GetExtent() const { return m_VkSwapChainExtent; }

	bool Initialize();
	void CreateSwapChain(Window* m_Window);
	void WaitForSync();

private:
	void CreateObjects();
	void TransitionImageLayout(
		uint32_t				imageIndex,
		vk::ImageLayout         old_layout,
		vk::ImageLayout         new_layout,
		vk::AccessFlags2        src_access_mask,
		vk::AccessFlags2        dst_access_mask,
		vk::PipelineStageFlags2 src_stage_mask,
		vk::PipelineStageFlags2 dst_stage_mask);

	void RecordCommandBuffer(uint32_t imageIndex, const vk::Pipeline& pipeline);

private:
	vk::raii::Context m_VkContext;
	vk::raii::Instance m_VkInstance{ nullptr };

	vk::ApplicationInfo m_VkApplicationInfo;
	vk::InstanceCreateInfo m_VkInstanceCreateInfo;
	vk::DeviceCreateInfo m_VkDeviceCreateInfo;

	vk::raii::PhysicalDevice m_VkPhysicalDevice{ nullptr };
	vk::raii::Device m_VkDevice{ nullptr };

	vk::Extent2D m_VkSwapChainExtent;

	vk::SurfaceFormatKHR m_VkSurfaceFormatKHR;
	vk::raii::SwapchainKHR m_VkSwapChain{ nullptr };

	std::vector<vk::raii::ImageView> m_VkSwapChainImageViews;
	std::vector<vk::Image> m_VkSwapChainImage;

	vk::raii::CommandPool m_VkCommandPool{ nullptr };
	vk::raii::CommandBuffer m_VkCommandBuffer{ nullptr };

	vk::raii::Semaphore m_VkPresentCompleteSemaphore = nullptr;
	vk::raii::Semaphore m_VkRenderFinishedSemaphore = nullptr;
	vk::raii::Fence     m_VkDrawFence = nullptr;

	vk::raii::Queue m_VkQueue{ nullptr };
};
}

#endif