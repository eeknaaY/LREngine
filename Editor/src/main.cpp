#include <LR/Engine.hpp>

int main() {
	LR::Engine engine = LR::Engine(LR::Core::API_TYPE::VULKAN);
	engine.CreateWindow(glm::vec2(850, 800), "");

	std::shared_ptr<LR::Shader> shader = engine.LoadShader("../../Engine/shaders/shader.spv");

	LR::GameObject obj = LR::GameObject();
	LR::GameObject obj2 = LR::GameObject();
	obj2.SetShader(shader);					// Eventually obj.SetComponent<Shader>() = shader;?
	obj.SetShader(shader);					// Eventually obj.SetComponent<Shader>() = shader;?
	
	std::vector<LR::Vertex> vertices = {
		{{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
	};

	std::vector<LR::Vertex> vertices2 = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{-1.0f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}
	};

	std::vector<uint16_t> indicies = { 0, 1, 2 };

	std::shared_ptr<LR::Mesh> mesh = engine.LoadMesh(vertices, indicies);
	std::shared_ptr<LR::Mesh> mesh2 = engine.LoadMesh(vertices2, indicies);

	obj.SetMesh(mesh);
	obj2.SetMesh(mesh2);

	LR::SubScene subScene = LR::SubScene();
	subScene.AddGameObject(obj);			// Eventually AddGameObject() to a Scene?
	subScene.AddGameObject(obj2);	


	while (engine.IsActive()) {
		engine.Draw(subScene);


		engine.Update();
	}

	return 0;
}