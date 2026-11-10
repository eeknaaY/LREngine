#ifndef LRSHADER_HPP
#define LRSHADER_HPP

#include <vulkan/vulkan_raii.hpp>

#include "Vulkan/VulkanShader.hpp"
#include "../Core.hpp"

namespace LR{

	class Shader {
	public:
		Shader() = default;
		Shader(API_TYPE Type, const char* path);
		Shader(API_TYPE Type, const char* path, const vk::raii::Device& device);
		~Shader();

		const bool Loaded;
	private:
		GenericShader* m_Shader = nullptr;

		bool LoadShader(API_TYPE Type, const char* path);
		bool LoadShader(API_TYPE Type, const char* path, const vk::raii::Device& device);
	};

}

#endif
