#ifndef LRDEBUG_HPP
#define LRDEBUG_HPP

#include <iostream>
#include <string_view>
#include <format>

namespace LR::Core {
	class Debug {
	public:
		static void Log(const std::string_view& message, const std::string_view& title = "");
		static void LogError(const std::string_view& message, const std::string_view& title = "");
	};
}

#endif