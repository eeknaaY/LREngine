#include "LR/Graphics/Vulkan/VulkanAPI.hpp"

#include <SDL3/SDL_vulkan.h>

namespace LR {


void VulkanAPI::Draw() {

}

bool VulkanAPI::Initialize() {
	m_VkApplicationInfo.pApplicationName = "Hello Triangle";
	m_VkApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	m_VkApplicationInfo.pEngineName = "No Engine";
	m_VkApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	m_VkApplicationInfo.apiVersion = VK_API_VERSION_1_0;

	m_VkInstanceCreateInfo.pApplicationInfo = &m_VkApplicationInfo;
	Uint32 extensionCount = 0;
	const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);
	m_VkInstanceCreateInfo.enabledExtensionCount = extensionCount;
	m_VkInstanceCreateInfo.ppEnabledExtensionNames = extensions;

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

	featureChain.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
		.extendedDynamicState = true;


	m_VkDeviceCreateInfo.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>();
	m_VkDeviceCreateInfo.queueCreateInfoCount = 1;
	m_VkDeviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
	m_VkDeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size());
	m_VkDeviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtension.data();

	m_VkDevice = vk::raii::Device(m_VkPhysicalDevice, m_VkDeviceCreateInfo);

	return true;
}

void VulkanAPI::CreateSwapChain(Window* m_Window) {
	vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_VkPhysicalDevice.getSurfaceCapabilitiesKHR(*m_Window);
	std::vector<vk::SurfaceFormatKHR> availableFormats = m_VkPhysicalDevice.getSurfaceFormatsKHR(*m_Window);
	std::vector<vk::PresentModeKHR> availablePresentModes = m_VkPhysicalDevice.getSurfacePresentModesKHR(*m_Window);

	const auto formatIt = std::ranges::find_if(
		availableFormats,
		[](const auto& format) { return format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; });

	vk::SwapchainCreateInfoKHR swapChainCreateInfo{};

	vk::SurfaceFormatKHR swapChainSurfaceFormat = formatIt != availableFormats.end() ? *formatIt : availableFormats[0];
	vk::Extent2D swapChainExtent{
		std::clamp<uint32_t>(m_Window->Size.x, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width),
		std::clamp<uint32_t>(m_Window->Size.y, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height)
	};

	swapChainCreateInfo.surface = *m_Window;
	swapChainCreateInfo.minImageCount = surfaceCapabilities.maxImageCount;
	swapChainCreateInfo.imageFormat = swapChainSurfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = swapChainSurfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = swapChainExtent;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
	swapChainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
	swapChainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	swapChainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
	swapChainCreateInfo.presentMode = vk::PresentModeKHR::eFifo;
	swapChainCreateInfo.clipped = true;
	swapChainCreateInfo.oldSwapchain = nullptr;

	m_SwapChain = vk::raii::SwapchainKHR(m_VkDevice, swapChainCreateInfo);
	m_SwapChainImages = m_SwapChain.getImages();

	vk::ImageViewCreateInfo imageViewCreateInfo;
	imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
	imageViewCreateInfo.format = swapChainSurfaceFormat.format;
	imageViewCreateInfo.subresourceRange = { vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 };
	imageViewCreateInfo.components = { vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity };

	/*for (auto& image : m_SwapChainImages)
	{
		imageViewCreateInfo.image = image;
		m_SwapChainImages.emplace_back(m_VkDevice, imageViewCreateInfo);
	}*/
}

}

