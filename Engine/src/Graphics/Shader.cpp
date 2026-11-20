#include "LR/Graphics/Shader.hpp"
#include "LR/Graphics/Vulkan/VulkanAPI.hpp"

#include <fstream>
#include <vector>
#include <iostream>


namespace LR {

	Shader::Shader(Core::API_TYPE Type, const char* path) : m_Shader(LoadShader(Type, path)), Loaded(m_Shader != nullptr), FileName(std::strrchr(path, '/') + 1) {
		std::cout << (Loaded ? std::format("{} loaded successfully.\n", FileName) : std::format("{} loading failed.\n", FileName));
	}

	Shader::Shader(Core::API_TYPE Type, const char* path, const vk::raii::Device& instance) : m_Shader(LoadShader(Type, path, instance)), Loaded(m_Shader != nullptr), FileName(std::strrchr(path, '/') + 1) {
		std::cout << (Loaded ? std::format("{} loaded successfully.\n", FileName) : std::format("{} loading failed.\n", FileName));
	}

	Shader::~Shader() {
		delete m_Shader;
	}

	void Shader::CacheShader(const std::shared_ptr<Shader>& shader) {	
		if (!shader) return;
		if (GetShader(shader->FileName)) return;

		m_LoadedShaders.insert(shader);
	}

	std::shared_ptr<Shader> Shader::GetShader(const char* fileName)
	{
		auto it = std::find_if(
			m_LoadedShaders.begin(),
			m_LoadedShaders.end(),
			[fileName](const std::shared_ptr<Shader>& shader) {
				return std::strcmp(shader->FileName, fileName) == 0;
			}
		);

		if (it != m_LoadedShaders.end()) {
			return *it;
		}
		else {
			return nullptr;
		}
	}

	GenericShader* Shader::LoadShader(Core::API_TYPE Type, const char* path)
	{
		try
		{
			std::ifstream file(path, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				throw std::runtime_error("Failed to open shader path.");
			}

			std::vector<char> buffer(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
			file.close();

			switch (Type) {
			case Core::API_TYPE::VULKAN:
				throw std::runtime_error("Vulkan shaders needs to be created using constructor with device as an arg.");
			default:
				throw std::runtime_error("Invalid/Unsupported graphics API selected for LRShader.");
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "ERROR: " << e.what() << '\n';
		}

		return nullptr;
	}

	GenericShader* Shader::LoadShader(Core::API_TYPE Type, const char* path, const vk::raii::Device& device)
	{
		try
		{
			std::ifstream file(path, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				throw std::runtime_error("Failed to open shader path.");
			}

			std::vector<char> buffer(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
			file.close();

			switch (Type) {
			case Core::API_TYPE::VULKAN:
				return new VulkanShader(buffer, device);

			default:
				throw std::runtime_error("Invalid/Unsupported graphics API selected for LRShader.");
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "ERROR: " << e.what() << '\n';
		}

		return nullptr;
	}



}

