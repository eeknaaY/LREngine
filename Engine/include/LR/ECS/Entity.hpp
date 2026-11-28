#ifndef LRENTITY_HPP
#define LRENTITY_HPP

#include <vector>
#include <string>

namespace LR {

	class Entity {
	public:
		Entity();
		~Entity();

		void AddComponent(const std::string& normalizedName) {
			m_Components.push_back(normalizedName);
		}

		uint64_t GetArchitectureID() const { return m_ArchitectureID; }
		void SetArchitectureID(uint64_t val) { m_ArchitectureID = val; }

		uint64_t GetIndex() const { return m_Index; }
		void SetIndex(uint64_t val) { m_Index = val; }

		const std::vector<std::string>& GetComponents() const { return m_Components; }

	private:
		std::vector<std::string> m_Components;

		uint64_t m_ArchitectureID = 0;
		uint64_t m_Index = UINT64_MAX;

	};
}


#endif