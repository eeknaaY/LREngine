#pragma once

#ifdef _WIN32
	#ifdef LRENGINE_EXPORTS
		#define LR_API __declspec(dllexport)
	#else
		#define LR_API __declspec(dllimport)
	#endif
#else
	#define LR_API
#endif