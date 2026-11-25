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

	void Draw(SubScene& subscene) override;

	const vk::raii::Device& GetDevice() const { return m_VkDevice; }
	const vk::raii::PhysicalDevice& GetPhysicalDevice() const { return m_VkPhysicalDevice; }
	const vk::SurfaceFormatKHR& GetSurfaceFormat() const { return m_VkSurfaceFormatKHR; }
	const vk::raii::Instance& GetInstance() const { return m_VkInstance; }
	const vk::Extent2D& GetExtent() const { return m_VkSwapChainExtent; }

	bool Initialize();
	void CreateSwapChain(Window* m_Window);
	void WaitForSync();

private:
	void InitializeObjects();
	void TransitionImageLayout(
		uint32_t				imageIndex,
		vk::ImageLayout         old_layout,
		vk::ImageLayout         new_layout,
		vk::AccessFlags2        src_access_mask,
		vk::AccessFlags2        dst_access_mask,
		vk::PipelineStageFlags2 src_stage_mask,
		vk::PipelineStageFlags2 dst_stage_mask);

	void RecordCommandBuffer(uint32_t imageIndex, const vk::Pipeline& pipeline, std::vector<std::shared_ptr<GameObject>>& objects);

private:
	const int MAX_FRAMES_IN_FLIGHT = 2;
	uint32_t m_FrameIndex = 0;
	
	vk::raii::Context m_VkContext;
	vk::raii::Instance m_VkInstance{ nullptr };

	vk::SurfaceFormatKHR m_VkSurfaceFormatKHR;

	vk::raii::PhysicalDevice m_VkPhysicalDevice{ nullptr };
	vk::raii::Device m_VkDevice{ nullptr };

	vk::raii::CommandPool m_VkCommandPool{ nullptr };

	std::vector<vk::raii::CommandBuffer> m_VkCommandBuffers;
	std::vector<vk::raii::Semaphore	> m_VkPresentCompleteSemaphores;
	std::vector<vk::raii::Semaphore	> m_VkRenderFinishedSemaphores;
	std::vector<vk::raii::Fence		> m_VkDrawFences;

	vk::raii::Queue m_VkQueue{ nullptr };

	vk::Extent2D m_VkSwapChainExtent;

	vk::raii::SwapchainKHR m_VkSwapChain{ nullptr };

	std::vector<vk::raii::ImageView> m_VkSwapChainImageViews;
	std::vector<vk::Image> m_VkSwapChainImage;
};
}

#endif