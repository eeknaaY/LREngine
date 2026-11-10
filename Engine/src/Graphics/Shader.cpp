#include "LR/Graphics/Shader.hpp"

#include <fstream>
#include <vector>
#include <iostream>

namespace LR {

	Shader::Shader(API_TYPE Type, const char* path) : Loaded(LoadShader(Type, path)) {

	}

	Shader::Shader(API_TYPE Type, const char* path, const vk::raii::Device& instance) : Loaded(LoadShader(Type, path, instance)) {

	}

	Shader::~Shader() {
		delete m_Shader;
	}

	bool Shader::LoadShader(API_TYPE Type, const char* path)
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
			case API_TYPE::VULKAN:
				throw std::runtime_error("Vulkan shaders needs to be created using constructor with instance as an arg.");
			default:
				throw std::runtime_error("Invalid/Unsupported graphics API selected for LRShader.");
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "ERROR: " << e.what() << '\n';
			return false;
		}

		return true;
	}

	bool Shader::LoadShader(API_TYPE Type, const char* path, const vk::raii::Device& device)
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
			case API_TYPE::VULKAN:
				m_Shader = new VulkanShader(buffer, device);
				break;
			default:
				throw std::runtime_error("Invalid/Unsupported graphics API selected for LRShader.");
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "ERROR: " << e.what() << '\n';
			return false;
		}

		return true;
	}



}

