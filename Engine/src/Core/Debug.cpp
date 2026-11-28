#include "LR/Core/Debug.hpp"

#include <iomanip>

namespace LR::Core {
	void Debug::Log(const std::string_view& message, const std::string_view& title) {
		if (title.size() == 0) {
			std::cout << message << "\n";
			return;
		}

		std::cout << std::left << std::setw(20) << title << message << "\n";
	}

	void Debug::LogError(const std::string_view& message, const std::string_view& title) {
		std::string redText = "\033[31mError: " + std::string(message) + "\033[0m\n";
		std::string redTitle = "\033[31mError: " + std::string(title) + "\033[0m\n";
		if (title.size() == 0) {
			std::cout << redText;
			return;
		}

		std::cout << std::left << std::setw(20) << redTitle << redText;
	}
}