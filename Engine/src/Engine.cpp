#include <LR/Engine.hpp>
#include <SDL3/SDL_vulkan.h>

namespace LR {

Engine::Engine(LR::API_TYPE Type) : m_API_Type(Type) {
	if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError()); // Load SDL

	m_Renderer = new Renderer(Type);
}

Engine::~Engine() {
	delete m_Window;
	delete m_Renderer;
}

void Engine::Update() {
	if (!m_Window) return;

	SDL_Event event;
	while (m_Window->PollEvents(&event)) {
		// eventhandler.signal(event) or something
		if (event.type == SDL_EVENT_QUIT) m_IsActive = false;
	}


}

Window* Engine::CreateWindow(const glm::vec2& size, const char* title, SDL_WindowFlags flags) {
	if (m_API_Type == LR::API_TYPE::VULKAN) flags |= SDL_WINDOW_VULKAN;
	m_Window = new Window(size, flags, title);
	
	switch (m_API_Type) {
	case LR::API_TYPE::VULKAN:
		VulkanAPI* vulkanAPI = static_cast<VulkanAPI*>(m_Renderer->GetGraphicsAPI());
		m_Window->CreateSurface(vulkanAPI->GetInstance());
		vulkanAPI->CreateSwapChain(m_Window);
		break;
	}


	return m_Window;
}

}