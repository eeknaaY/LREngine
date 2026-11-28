#include "LR/Graphics/Vulkan/VulkanAPI.hpp"

#include <SDL3/SDL_vulkan.h>
#include <iostream>

namespace LR {

VulkanAPI::VulkanAPI() {

}

VulkanAPI::~VulkanAPI() {

}


void VulkanAPI::Draw(SubScene& subscene) {
	m_FrameIndex = (m_FrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;

	std::vector<std::shared_ptr<GameObject>>& objects = subscene.GetGameObjets();
	if (objects.size() == 0) return;

	// yah really fucked atm, just for testing
	VulkanShader* shader = objects[0]->GetShader()->Get<VulkanShader>();
	vk::Pipeline pipeline = *shader->GetPipeline();

	auto fenceResult = m_VkDevice.waitForFences(*m_VkDrawFences[m_FrameIndex], vk::True, UINT64_MAX);
	if (fenceResult != vk::Result::eSuccess)
	{
		Core::Debug::LogError("Failed to wait for fence.");
		return;
	}

	auto [result, imageIndex] = m_VkSwapChain.acquireNextImage(UINT64_MAX, *m_VkPresentCompleteSemaphores[m_FrameIndex], nullptr);
	
	m_VkCommandBuffers[m_FrameIndex].reset();
	m_VkDevice.resetFences(*m_VkDrawFences[m_FrameIndex]);

	RecordCommandBuffer(imageIndex, pipeline, objects);

	vk::PipelineStageFlags waitDestinationStageMask(
		vk::PipelineStageFlagBits::eColorAttachmentOutput
	);

	vk::SubmitInfo submitInfo{};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &*m_VkPresentCompleteSemaphores[m_FrameIndex];
	submitInfo.pWaitDstStageMask = &waitDestinationStageMask;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &*m_VkCommandBuffers[m_FrameIndex];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &*m_VkRenderFinishedSemaphores[imageIndex];

	m_VkQueue.submit(submitInfo, *m_VkDrawFences[m_FrameIndex]);

	vk::PresentInfoKHR presentInfoKHR{};
	presentInfoKHR.waitSemaphoreCount = 1;
	presentInfoKHR.pWaitSemaphores = &*m_VkRenderFinishedSemaphores[imageIndex];
	presentInfoKHR.swapchainCount = 1;
	presentInfoKHR.pSwapchains = &*m_VkSwapChain;
	presentInfoKHR.pImageIndices = &imageIndex;

	result = m_VkQueue.presentKHR(presentInfoKHR);
}

bool VulkanAPI::Initialize() {
	InitializeObjects();

	return true;
}

void VulkanAPI::InitializeObjects() {
	vk::ApplicationInfo m_VkApplicationInfo;
	m_VkApplicationInfo.pApplicationName = "Hello Triangle";
	m_VkApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	m_VkApplicationInfo.pEngineName = "No Engine";
	m_VkApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	m_VkApplicationInfo.apiVersion = VK_API_VERSION_1_3;

	vk::InstanceCreateInfo m_VkInstanceCreateInfo;
	m_VkInstanceCreateInfo.pApplicationInfo = &m_VkApplicationInfo;
	Uint32 extensionCount = 0;
	const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
	m_VkInstanceCreateInfo.enabledExtensionCount = extensionCount;
	m_VkInstanceCreateInfo.ppEnabledExtensionNames = extensions;

	// ++DEBUG
	std::vector<const char*> extensionVector(extensions, extensions + extensionCount);
	const std::vector<char const*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
	constexpr bool enableValidationLayers = true;
	// Get the required layers
	std::vector<char const*> requiredLayers;
	if (enableValidationLayers)
	{
		requiredLayers.assign(validationLayers.begin(), validationLayers.end());
	}

	// Check if the required layers are supported by the Vulkan implementation.
	auto layerProperties = m_VkContext.enumerateInstanceLayerProperties();
	auto unsupportedLayerIt = std::ranges::find_if(requiredLayers,
		[&layerProperties](auto const& requiredLayer) {
			return std::ranges::none_of(layerProperties,
				[requiredLayer](auto const& layerProperty) { return strcmp(layerProperty.layerName, requiredLayer) == 0; });
		});
	if (unsupportedLayerIt != requiredLayers.end())
	{
		throw std::runtime_error("Required layer not supported: " + std::string(*unsupportedLayerIt));
	}

	// Check if the required extensions are supported by the Vulkan implementation.
	auto extensionProperties = m_VkContext.enumerateInstanceExtensionProperties();
	auto unsupportedPropertyIt =
		std::ranges::find_if(extensionVector,
			[&extensionProperties](auto const& requiredExtension) {
				return std::ranges::none_of(extensionProperties,
					[requiredExtension](auto const& extensionProperty) { return strcmp(extensionProperty.extensionName, requiredExtension) == 0; });
			});
	if (unsupportedPropertyIt != extensionVector.end())
	{
		throw std::runtime_error("Required extension not supported: " + std::string(*unsupportedPropertyIt));
	}

	m_VkInstanceCreateInfo.sType = vk::StructureType::eInstanceCreateInfo;
	m_VkInstanceCreateInfo.pApplicationInfo = &m_VkApplicationInfo;
	m_VkInstanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
	m_VkInstanceCreateInfo.ppEnabledLayerNames = requiredLayers.data();
	m_VkInstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensionVector.size());
	m_VkInstanceCreateInfo.ppEnabledExtensionNames = extensionVector.data();
	// --DEBUG


	m_VkInstanceCreateInfo.pApplicationInfo = &m_VkApplicationInfo;

	m_VkInstance = vk::raii::Instance(m_VkContext, m_VkInstanceCreateInfo);
	m_VkPhysicalDevice = m_VkInstance.enumeratePhysicalDevices().front();

	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = m_VkPhysicalDevice.getQueueFamilyProperties();
	auto graphicsQueueFamilyProperty = std::ranges::find_if(queueFamilyProperties, [](auto const& qfp) { return (qfp.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0); });
	auto graphicsIndex = static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));

	std::vector<const char*> requiredDeviceExtension = { vk::KHRSwapchainExtensionName };

	float queuePriority = 0.5f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo;
	deviceQueueCreateInfo.queueFamilyIndex = graphicsIndex;
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

	vk::StructureChain<
		vk::PhysicalDeviceFeatures2,
		vk::PhysicalDeviceVulkan11Features,
		vk::PhysicalDeviceVulkan13Features,
		vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
	> featureChain;

	featureChain.get<vk::PhysicalDeviceVulkan11Features>()
		.shaderDrawParameters = true;

	featureChain.get<vk::PhysicalDeviceVulkan13Features>()
		.dynamicRendering = true;

	featureChain.get<vk::PhysicalDeviceVulkan13Features>()
		.synchronization2 = true;

	featureChain.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
		.extendedDynamicState = true;

	vk::DeviceCreateInfo m_VkDeviceCreateInfo;
	m_VkDeviceCreateInfo.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>();
	m_VkDeviceCreateInfo.queueCreateInfoCount = 1;
	m_VkDeviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	m_VkDeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size());
	m_VkDeviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtension.data();

	m_VkDevice = vk::raii::Device(m_VkPhysicalDevice, m_VkDeviceCreateInfo);
	m_VkQueue = m_VkDevice.getQueue(graphicsIndex, 0);

	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
	poolInfo.queueFamilyIndex = graphicsIndex;

	m_VkCommandPool = vk::raii::CommandPool(m_VkDevice, poolInfo);

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = m_VkCommandPool;
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

	m_VkCommandBuffers = vk::raii::CommandBuffers(m_VkDevice, allocInfo);
}

void VulkanAPI::CreateSwapChain(Window* m_Window) {
	vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_VkPhysicalDevice.getSurfaceCapabilitiesKHR(*m_Window);
	std::vector<vk::SurfaceFormatKHR> availableFormats = m_VkPhysicalDevice.getSurfaceFormatsKHR(*m_Window);
	std::vector<vk::PresentModeKHR> availablePresentModes = m_VkPhysicalDevice.getSurfacePresentModesKHR(*m_Window);

	const auto formatIt = std::ranges::find_if(
		availableFormats,
		[](const auto& format) { return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; });

	vk::SwapchainCreateInfoKHR swapChainCreateInfo{};

	m_VkSurfaceFormatKHR = formatIt != availableFormats.end() ? *formatIt : availableFormats[0];
	m_VkSwapChainExtent = vk::Extent2D{
		std::clamp<uint32_t>(m_Window->Size.x, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width),
		std::clamp<uint32_t>(m_Window->Size.y, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height)
	};

	swapChainCreateInfo.surface = *m_Window;
	swapChainCreateInfo.minImageCount = surfaceCapabilities.maxImageCount;
	swapChainCreateInfo.imageFormat = m_VkSurfaceFormatKHR.format;
	swapChainCreateInfo.imageColorSpace = m_VkSurfaceFormatKHR.colorSpace;
	swapChainCreateInfo.imageExtent = m_VkSwapChainExtent;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
	swapChainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
	swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	swapChainCreateInfo.presentMode = vk::PresentModeKHR::eFifo;
	swapChainCreateInfo.clipped = true;
	swapChainCreateInfo.oldSwapchain = nullptr;

	m_VkSwapChain = vk::raii::SwapchainKHR(m_VkDevice, swapChainCreateInfo);
	m_VkSwapChainImage = m_VkSwapChain.getImages();

	vk::ImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
	imageViewCreateInfo.format = m_VkSurfaceFormatKHR.format;
	imageViewCreateInfo.subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };
	imageViewCreateInfo.components = { vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity };

	for (auto& image : m_VkSwapChainImage)
	{
		imageViewCreateInfo.image = image;
		m_VkSwapChainImageViews.emplace_back(
			m_VkDevice.createImageView(imageViewCreateInfo)
		);
	}

	vk::FenceCreateInfo fenceInfo{};
	fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

	for (size_t i = 0; i < m_VkSwapChainImage.size(); i++)
	{
		m_VkRenderFinishedSemaphores.emplace_back(m_VkDevice, vk::SemaphoreCreateInfo());
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		m_VkPresentCompleteSemaphores.emplace_back(m_VkDevice, vk::SemaphoreCreateInfo());
		m_VkDrawFences.emplace_back(m_VkDevice, fenceInfo);
	}
}

void VulkanAPI::WaitForSync() {

}

void VulkanAPI::TransitionImageLayout(
	uint32_t                imageIndex,
	vk::ImageLayout         old_layout,
	vk::ImageLayout         new_layout,
	vk::AccessFlags2        src_access_mask,
	vk::AccessFlags2        dst_access_mask,
	vk::PipelineStageFlags2 src_stage_mask,
	vk::PipelineStageFlags2 dst_stage_mask)
{

	vk::ImageMemoryBarrier2 barrier{};

	barrier.srcStageMask = src_stage_mask;
	barrier.srcAccessMask = src_access_mask;
	barrier.dstStageMask = dst_stage_mask;
	barrier.dstAccessMask = dst_access_mask;
	barrier.oldLayout = old_layout;
	barrier.newLayout = new_layout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = m_VkSwapChainImage[imageIndex];

	barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	vk::DependencyInfo dependency_info{};

	dependency_info.dependencyFlags = {};
	dependency_info.imageMemoryBarrierCount = 1;
	dependency_info.pImageMemoryBarriers = &barrier;
	m_VkCommandBuffers[m_FrameIndex].pipelineBarrier2(dependency_info);
}

void VulkanAPI::RecordCommandBuffer(uint32_t imageIndex, const vk::Pipeline& pipeline, std::vector<std::shared_ptr<GameObject>>& objects) {
	vk::raii::CommandBuffer& commandBuffer = m_VkCommandBuffers[m_FrameIndex];
	commandBuffer.begin({});

	// Before starting rendering, transition the swapchain image to vk::ImageLayout::eColorAttachmentOptimal
	TransitionImageLayout(
		imageIndex,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::eColorAttachmentOptimal,
		{},                                                        // srcAccessMask (no need to wait for previous operations)
		vk::AccessFlagBits2::eColorAttachmentWrite,                // dstAccessMask
		vk::PipelineStageFlagBits2::eColorAttachmentOutput,        // srcStage
		vk::PipelineStageFlagBits2::eColorAttachmentOutput         // dstStage
	);


	vk::ClearValue clearColor = vk::ClearColorValue(
		0.0f, 0.0f, 0.0f, 1.0f
	);

	vk::RenderingAttachmentInfo attachmentInfo{};

	attachmentInfo.imageView = m_VkSwapChainImageViews[imageIndex];
	attachmentInfo.imageLayout = vk::ImageLayout::eColorAttachmentOptimal;
	attachmentInfo.loadOp = vk::AttachmentLoadOp::eClear;
	attachmentInfo.storeOp = vk::AttachmentStoreOp::eStore;
	attachmentInfo.clearValue = clearColor;

	vk::RenderingInfo renderingInfo{};
	renderingInfo.renderArea.offset = vk::Offset2D{ 0, 0 };
	renderingInfo.renderArea.extent = m_VkSwapChainExtent;
	renderingInfo.layerCount = 1;
	renderingInfo.colorAttachmentCount = 1;
	renderingInfo.pColorAttachments = &attachmentInfo;

	commandBuffer.beginRendering(renderingInfo);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

	commandBuffer.setViewport(0, vk::Viewport(0.0f, 0.0f, static_cast<float>(m_VkSwapChainExtent.width), static_cast<float>(m_VkSwapChainExtent.height), 0.0f, 1.0f));
	commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_VkSwapChainExtent));

	for (std::shared_ptr<GameObject>& obj : objects) {
		std::shared_ptr<Mesh> mesh = obj->GetMesh();
		VulkanMesh* vmesh = mesh->Get<VulkanMesh>();

		commandBuffer.bindVertexBuffers(0, *vmesh->m_VertexBuffer, {0});
		commandBuffer.bindIndexBuffer(*vmesh->m_IndexBuffer, 0, vk::IndexType::eUint16);
		commandBuffer.drawIndexed(static_cast<uint32_t>(mesh->GetIndices().size()), 1, 0, 0, 0);
	}

	commandBuffer.endRendering();

	// After rendering, transition the swapchain image to vk::ImageLayout::ePresentSrcKHR
	TransitionImageLayout(
		imageIndex,
		vk::ImageLayout::eColorAttachmentOptimal,
		vk::ImageLayout::ePresentSrcKHR,
		vk::AccessFlagBits2::eColorAttachmentWrite,             // srcAccessMask
		{},                                                     // dstAccessMask
		vk::PipelineStageFlagBits2::eColorAttachmentOutput,     // srcStage
		vk::PipelineStageFlagBits2::eBottomOfPipe               // dstStage
	);

	commandBuffer.end();
}

}

