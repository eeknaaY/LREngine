#include <iostream>
#include <LR/Engine.hpp>

int main() {
	LR::Engine engine = LR::Engine(LR::Core::API_TYPE::VULKAN);
	engine.CreateWindow(glm::vec2(850, 800), "");

	std::shared_ptr<LR::Shader> shader = engine.LoadShader("../../Engine/shaders/shader.spv");

	LR::GameObject obj = LR::GameObject();
	obj.SetShader(shader);					// Eventually obj.SetComponent<Shader>() = shader;?
	
	LR::Mesh mesh = LR::Mesh();
	LR::Vertex vertex { .Position = glm::vec3(0, 0, 0) };
	std::vector<LR::Vertex> vertices = { vertex };

	mesh.SetVertices(vertices);
	obj.SetMesh(mesh);

	LR::SubScene subScene = LR::SubScene();
	subScene.AddGameObject(obj);			// Eventually AddGameObject() to a Scene?

	while (engine.IsActive()) {
		engine.Draw(subScene);


		engine.Update();
	}

	return 0;
}