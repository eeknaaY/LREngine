#include <LR/Engine.hpp>
#include <SDL3/SDL_vulkan.h>

namespace LR {

Engine::Engine(LR::API_TYPE Type) : m_API_Type(Type) {
	if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError()); // Load SDL

	m_Renderer = std::make_shared<Renderer>(Type);
}

Engine::~Engine() {

}

void Engine::Update() {
	SDL_Event event;
	while (m_Window->PollEvents(&event)) {
		// eventhandler.signal(event) or something
		if (event.type == SDL_EVENT_QUIT) m_IsActive = false;
	}


}

std::shared_ptr<Window> Engine::CreateWindow(const glm::vec2& size, const char* title, SDL_WindowFlags flags) {
	if (m_API_Type == LR::API_TYPE::VULKAN) flags |= SDL_WINDOW_VULKAN;
	m_Window = std::make_shared<Window>(size, flags, title);
	
	switch (m_API_Type) {
	case LR::API_TYPE::VULKAN:
		VulkanAPI* vulkanAPI = static_cast<VulkanAPI*>(m_Renderer->GetGraphicsAPI());
		m_Window->CreateSurface(vulkanAPI->GetInstance());
		vulkanAPI->CreateSwapChain(m_Window.get());
		break;
	}


	return m_Window;
}

Shader Engine::LoadShader(const char* path)
{
	switch (m_API_Type) {
	case LR::API_TYPE::VULKAN:
		VulkanAPI* vulkanAPI = static_cast<VulkanAPI*>(m_Renderer->GetGraphicsAPI());
		return Shader(m_API_Type, path, vulkanAPI->GetDevice());
	}

	return Shader(m_API_Type, path);
}

}