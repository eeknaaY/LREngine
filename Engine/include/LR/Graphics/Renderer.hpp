#ifndef LRRENDERER_HPP
#define LRRENDERER_HPP

#include "Vulkan/VulkanAPI.hpp"
#include "../Scene/SubScene.hpp"
#include "../Core/Core.hpp"


namespace LR {

// Factory for rendering APIs
class Renderer {
public:
	Renderer() = default;
	Renderer(Core::API_TYPE Type);
	~Renderer();

	void Draw(SubScene& obj);

	template <typename T> requires std::derived_from<T, GraphicsAPI>
	T* Get() { return static_cast<T*>(m_GraphicsAPI); }

private:
	GraphicsAPI* m_GraphicsAPI = nullptr;

	bool Initialize();
};

}

#endif