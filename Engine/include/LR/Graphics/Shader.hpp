#ifndef LRSHADER_HPP
#define LRSHADER_HPP

#include <vulkan/vulkan_raii.hpp>

#include "Vulkan/VulkanShader.hpp"
#include "../Core/Core.hpp"


#include <concepts>
#include <unordered_set>

namespace LR{

	class Shader {
	public:
		Shader() = default;
		Shader(Core::API_TYPE Type, const char* path);
		Shader(Core::API_TYPE Type, const char* path, const vk::raii::Device& device);
		~Shader();

		static void CacheShader(const std::shared_ptr<Shader>& shader);
		/*
		* Retrives cached shader.
		* 
		* @param name Name of the shader file (NOT PATH!), including the extention.
		* 
		* @return Shared Pointer to the Shader.
		*/
		static std::shared_ptr<Shader> GetShader(const char* name);

		template <typename T> requires std::derived_from<T, GenericShader>
		T* Get() { return static_cast<T*>(m_Shader); }

		const bool Loaded;
		const char* FileName;
	
	private:
		GenericShader* LoadShader(Core::API_TYPE Type, const char* path);
		GenericShader* LoadShader(Core::API_TYPE Type, const char* path, const vk::raii::Device& device);
		

		inline static std::unordered_set<std::shared_ptr<Shader>>m_LoadedShaders;
		GenericShader* m_Shader = nullptr;
	};

}

#endif
