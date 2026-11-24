#include <LR/Engine.hpp>
#include <SDL3/SDL_vulkan.h>

namespace LR {

Engine::Engine(Core::API_TYPE Type) {
	try {
		if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError()); // Load SDL

		m_Renderer = std::make_shared<Renderer>(Type);
		m_Camera = std::make_shared<Camera>();
		Core::settings.SetAPIUsage(Type);
		Core::Debug::Log(std::format("Using API: {}", Core::APIToString(Type)));
	} 
	catch (const std::exception& e) {
		Core::settings.SetAPIUsage(Core::API_TYPE::HEADLESS);
		Core::Debug::LogError("Unable to create SDL window. Proceeding as headless.");
	}
}

Engine::~Engine() {

}

void Engine::Update() {
	SDL_Event event;
	while (m_Window->PollEvents(&event)) {
		// eventhandler.signal(event) or something
		if (event.type == SDL_EVENT_QUIT) m_IsActive = false;
	}

	//if (m_Camera) m_Camera->Update();
}

std::shared_ptr<Window> Engine::CreateWindow(const glm::vec2& size, const char* title, SDL_WindowFlags flags) {
	LR::Core::API_TYPE m_API_Type = LR::Core::settings.GetAPIUsage();
	if (m_API_Type == LR::Core::API_TYPE::VULKAN) flags |= SDL_WINDOW_VULKAN;
	m_Window = std::make_shared<Window>(size, flags, title);
	
	switch (m_API_Type) {
	case LR::Core::API_TYPE::VULKAN:
		VulkanAPI* vulkanAPI = static_cast<VulkanAPI*>(m_Renderer->GetGraphicsAPI());
		m_Window->CreateSurface(vulkanAPI->GetInstance());
		vulkanAPI->CreateSwapChain(m_Window.get());
		break;
	}


	return m_Window;
}

// Send in the path of the shader, TODO: Atm requires a "/" before the name, fix, the Shader FileName is also done like this, fix as well
std::shared_ptr<Shader> Engine::LoadShader(const char* path) {
	
	
	const char* fileName = std::strrchr(path, '/') + 1;
	std::shared_ptr<Shader> shader;

	// For now return the shader if its already loaded, TODO: You should be able to create new instances of the same shader, will have to return a list of ptrs.
	shader = Shader::GetShader(fileName);
	if (shader) {
		Core::Debug::Log(std::format("Retrieving cached shader: {}", fileName));
		return shader;
	}

	Core::API_TYPE apiType = Core::settings.GetAPIUsage();
	switch (apiType) {
	case Core::API_TYPE::VULKAN:
		VulkanAPI* vulkanAPI = static_cast<VulkanAPI*>(m_Renderer->GetGraphicsAPI());
		shader = std::make_shared<Shader>(apiType, path, vulkanAPI->GetDevice());
		if (shader->Loaded) {
			VulkanShader* vulkanShader = shader->Get<VulkanShader>();
			vulkanShader->CreatePipeline(vulkanAPI->GetExtent(), vulkanAPI->GetDevice(), vulkanAPI->GetSurfaceFormat()); // TODO: Make selectable pre-created pipeline.
		}
		break;
	}

	if (!shader) shader = std::make_shared<Shader>(apiType, path);
	Core::Debug::Log(std::format("Loading new shader: {}", fileName));

	Shader::CacheShader(shader);

	return shader;
}

std::shared_ptr<Shader> Engine::GetShader(const char* fileName)
{
	return Shader::GetShader(fileName);
}

void Engine::Draw(const SubScene& obj) {
	m_Renderer->Draw(obj);
}

}