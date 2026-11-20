#ifndef LRSETTINGS_HPP
#define LRSETTINGS_HPP

#include <stdexcept>

namespace LR {
	namespace Core {
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
	}
}

#endif