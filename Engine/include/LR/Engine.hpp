#ifndef LRENGINE_HPP
#define LRENGINE_HPP

#include "Export.hpp"

#include "Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Mesh.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Camera.hpp"
#include "Core/Core.hpp"

namespace LR {

class Engine {
public:
	Engine(LR::Core::API_TYPE Type);
	~Engine();
	
	void Update();
	bool IsActive() const { return m_IsActive; }

	std::shared_ptr<Window> CreateWindow(const glm::vec2& size, const char* title, SDL_WindowFlags flags = 0);

	std::shared_ptr<Mesh> LoadMesh(const std::vector<Vertex>& Vertices, const std::vector<uint16_t>& Indicies);

	std::shared_ptr<Shader> LoadShader(const char* path);
	std::shared_ptr<Shader> GetShader(const char* path);
	
	void Draw(SubScene& obj);

private:

	bool m_IsActive = true;

	std::shared_ptr<Renderer>m_Renderer;
	std::shared_ptr<Window>m_Window;
	std::shared_ptr<Camera>m_Camera;

	//std::unordered_map<std::shared_ptr<Shader>, std::vector<GameObject>>m_ShaderObjects; // Should be in subscene
};

}



#endif