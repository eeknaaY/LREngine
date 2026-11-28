#include <LR/Engine.hpp>

int main() {
	LR::Engine engine = LR::Engine(LR::Core::API_TYPE::VULKAN);
	engine.CreateWindow(glm::vec2(850, 800), "");

	std::shared_ptr<LR::Shader> shader = engine.LoadShader("../../Engine/shaders/shader.spv");

	LR::GameObject obj = LR::GameObject();
	obj.SetShader(shader);					// Eventually obj.SetComponent<Shader>() = shader;?
	
	const std::vector<LR::Vertex> vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	std::shared_ptr<LR::Mesh> mesh = engine.LoadMesh(vertices, indices);

	obj.SetMesh(mesh);

	LR::SubScene subScene = LR::SubScene();
	subScene.AddGameObject(obj);			// Eventually AddGameObject() to a Scene?

	std::shared_ptr<LR::Components::Transform> transform = obj.AddComponent<LR::Components::Transform>();
	transform->Position = glm::vec3(1, 0, 0);

	// TODO: Currently only 1 component architecture can exist, fix
	// TODO: Vulkan buffer pre-write buffer thingies
	std::shared_ptr<LR::Components::Transform> transform_ = obj.GetComponent<LR::Components::Transform>();
	LR::Core::Debug::Log(std::format("Position Value ->\t{}", transform_->Position.x));

	while (engine.IsActive()) {
		engine.Draw(subScene);



		engine.Update();
	}

	return 0;
}