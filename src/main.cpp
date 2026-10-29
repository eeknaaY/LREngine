#include <iostream>
#include <vulkan/vulkan.h>

int main() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "LREngine";
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    VkInstance instance = VK_NULL_HANDLE;

    VkResult result = vkCreateInstance(
        &createInfo,
        nullptr,
        &instance
    );

    if (result != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance. Error: "
            << result << '\n';
        return 1;
    }

    std::cout << "Vulkan initialized successfully!\n";

    vkDestroyInstance(instance, nullptr);
    return 0;
}