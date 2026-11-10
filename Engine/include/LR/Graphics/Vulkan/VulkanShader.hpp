#ifndef LRVULKANSHADER_HPP
#define LRVULKANSHADER_HPP

#include "../Interfaces/GenericShader.hpp"
#include "../../Window.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace LR {
	class VulkanShader : public GenericShader {
	public:
		VulkanShader(const std::vector<char>& buffer, const vk::raii::Device& device);
		~VulkanShader() override;

		void Bind() override;

	private:
		vk::raii::ShaderModule m_Shader{ nullptr };
	};
}

#endif