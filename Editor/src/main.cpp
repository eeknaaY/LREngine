#include <iostream>
#include <LREngine.hpp>

int main() {
	LREngine engine = LREngine(LR::API_TYPE::VULKAN);
	engine.CreateWindow(glm::vec2(1200, 800), SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE, "");

	while (engine.IsActive()) {
		

		engine.Update();
	}

	return 0;
}