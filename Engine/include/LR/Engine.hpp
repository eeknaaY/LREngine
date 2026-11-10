#ifndef LRENGINE_HPP
#define LRENGINE_HPP

#include "Export.hpp"

#include "Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"

namespace LR {

class Engine {
public:
	Engine(LR::API_TYPE Type);
	~Engine();

	void Update();
	bool IsActive() const { return m_IsActive; }

	std::shared_ptr<Window> CreateWindow(const glm::vec2& size, const char* title, SDL_WindowFlags flags = 0);
	Shader LoadShader(const char* path);

private:
	bool m_IsActive = true;

	std::shared_ptr<Window>m_Window;
	std::shared_ptr<Renderer>m_Renderer;
	// Load shaders into a list?
	//std::vector<Shader> m_Shaders;

	const LR::API_TYPE m_API_Type;
};

}



#endif