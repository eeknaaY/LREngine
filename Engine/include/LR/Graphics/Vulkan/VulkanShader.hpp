#ifndef LRVULKANSHADER_HPP
#define LRVULKANSHADER_HPP

#include "../Interfaces/GenericShader.hpp"
#include "../../Window.hpp"

#include <glm/vec2.hpp>
#include <vulkan/vulkan_raii.hpp>

namespace LR {
	class VulkanShader : public GenericShader {
	public:
		VulkanShader(const std::vector<char>& buffer, const vk::raii::Device& device);
		~VulkanShader() override;

		vk::raii::Pipeline* GetPipeline() { return &m_Pipeline; }
		const vk::raii::ShaderModule& GetModule() const { return m_ShaderModule; }

		void Bind() override;

		// Dont really like having 3 args instead of just parsing VulkanAPI*, but I cba with a circular dependancy
		void CreatePipeline(const vk::Extent2D surfaceExtent, const vk::raii::Device& device, const vk::SurfaceFormatKHR& swapChainSurfaceFormat);
	private:

	private:
		vk::raii::ShaderModule m_ShaderModule{ nullptr };
		vk::raii::Pipeline m_Pipeline{ nullptr };
	};
}

#endif