#include <iostream>
#include <LR/Engine.hpp>

int main() {
	LR::Engine engine = LR::Engine(LR::API_TYPE::VULKAN);
	engine.CreateWindow(glm::vec2(1200, 800), "");

	LR::Shader shader = engine.LoadShader("../../Engine/shaders/shader.spv");

	std::cout << shader.Loaded;

	while (engine.IsActive()) {

		engine.Update();
	}

	return 0;
}