#include <LR/Graphics/Renderer.hpp>

namespace LR {

Renderer::Renderer(Core::API_TYPE Type) {
	switch (Type) {
	case Core::API_TYPE::VULKAN:
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

void Renderer::Draw(const SubScene& subscene) {
	m_GraphicsAPI->Draw(subscene);
}

bool Renderer::Initialize() {
	return m_GraphicsAPI->Initialize();
}

}