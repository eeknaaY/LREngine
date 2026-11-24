#ifndef LRDEBUG_HPP
#define LRDEBUG_HPP

#include <iostream>
#include <string_view>

namespace LR::Core {
	class Debug {
	public:
		static void Log(const std::string_view& message);
		static void LogError(const std::string_view& message);
	};
}

#endif