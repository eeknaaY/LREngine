#ifndef LRSETTINGS_HPP
#define LRSETTINGS_HPP

#include <stdexcept>

namespace LR::Core {
		enum class API_TYPE {
			UNINITIALIZED,
			VULKAN,
			HEADLESS
		};

		class Settings {
		public:
			const API_TYPE GetAPIUsage() const { return m_API_TYPE; }
			void SetAPIUsage(API_TYPE API) {
				if (m_API_TYPE != API_TYPE::UNINITIALIZED && API != m_API_TYPE) {
					throw std::runtime_error("Attempted to change API when already set.");
				}

				m_API_TYPE = API;
			}



		private:
			API_TYPE m_API_TYPE = API_TYPE::UNINITIALIZED;
		};

		extern Settings settings;

		inline std::string APIToString(API_TYPE API) {
			switch (API) {
			case API_TYPE::UNINITIALIZED: return "UNINITIALIZED";
			case API_TYPE::VULKAN:        return "VULKAN";
			case API_TYPE::HEADLESS:      return "HEADLESS";
			default:                      return "UNKNOWN";
			}
		}
}

#endif