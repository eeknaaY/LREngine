#include <LR/Graphics/Renderer.hpp>

namespace LR {

Renderer::Renderer(LR::API_TYPE Type) {
	switch (Type) {
	case LR::API_TYPE::VULKAN:
		m_GraphicsAPI = new VulkanAPI();
		break;

	default:
		throw std::runtime_error("Invalid/Unsupported graphics API selected for LRRenderer.");
		return;
	}

	Initialize();
}

Renderer::~Renderer() {
	delete m_GraphicsAPI;
}

bool Renderer::Initialize() {
	return m_GraphicsAPI->Initialize();
}

}