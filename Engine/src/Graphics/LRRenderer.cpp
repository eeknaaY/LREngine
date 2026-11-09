#include "Graphics/LRRenderer.hpp"

LRRenderer::LRRenderer(LR::API_TYPE Type) {
	switch (Type) {
	case LR::API_TYPE::VULKAN:
		m_GraphicsAPI = new LRVulkanAPI();
		break;

	default:
		return;
	}

	Initialize();
}

LRRenderer::~LRRenderer() {
	delete m_GraphicsAPI;
}

bool LRRenderer::Initialize() {
	return m_GraphicsAPI->Initialize();
}