#ifndef LRRENDERER_HPP
#define LRRENDERER_HPP

#include "Vulkan/VulkanAPI.hpp"
#include "../Core.hpp"


namespace LR {

// Factory for rendering APIs
class Renderer {
public:
	Renderer() = default;
	Renderer(API_TYPE Type);
	~Renderer();

	GraphicsAPI* GetGraphicsAPI() const { return m_GraphicsAPI; }

private:
	GraphicsAPI* m_GraphicsAPI = nullptr;

	bool Initialize();
};

}

#endif