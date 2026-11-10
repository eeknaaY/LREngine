#include <LR/Graphics/Vulkan/VulkanShader.hpp>

namespace LR {
	VulkanShader::VulkanShader(const std::vector<char>& buffer, const vk::raii::Device& device) {
		vk::ShaderModuleCreateInfo createInfo{};
		createInfo.codeSize = buffer.size() * sizeof(char);
		createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

		m_Shader = vk::raii::ShaderModule{ device, createInfo };
	}

	VulkanShader::~VulkanShader() {

	}

	void VulkanShader::Bind() {

	}
}

