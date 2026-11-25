#include "LR/Core/Debug.hpp"

namespace LR::Core {
	void Debug::Log(const std::string_view& message) {
		std::cout << message << "\n";
	}

	void Debug::LogError(const std::string_view& message) {
		std::string redText = "\033[31mError: " + std::string(message) + "\033[0m\n";
		std::cout << redText;
	}
}