#ifndef LRENGINE_HPP
#define LRENGINE_HPP

#include "Export.hpp"

#include "Window.hpp"
#include "Graphics/Renderer.hpp"

namespace LR {

class Engine {
public:
	Engine(LR::API_TYPE Type);
	~Engine();

	void Update();
	bool IsActive() const { return m_IsActive; }

	Window* CreateWindow(const glm::vec2& size, const char* title, SDL_WindowFlags flags = 0);

private:
	bool m_IsActive = true;
	Window* m_Window = nullptr;
	Renderer* m_Renderer = nullptr;
	const LR::API_TYPE m_API_Type;

};

}



#endif