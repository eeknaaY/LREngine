#ifndef LRENTITYMANAGER_HPP
#define LRENTITYMANAGER_HPP

#include "ComponentRegistry.hpp"
#include "Entity.hpp"

#include <memory>

namespace LR {

	class EntityManager {
	public:
		template <typename T> requires std::derived_from<T, Components::Component>
		std::shared_ptr<T> AddComponent(Entity& entity) {
			if (entity.GetIndex() == UINT64_MAX) {
				entity.AddComponent(ComponentRegistry::NormalizeName(typeid(T).name()));
				entity.SetArchitectureID(m_ComponentRegistry.CalculateArchitectureID(entity));
			} else {
				// Object has been initialized, need to remove it from the other architecture.
			}

			return m_ComponentRegistry.AddComponent<T>(entity);
		}

		template <typename T> requires std::derived_from<T, Components::Component>
		std::shared_ptr<T> GetComponent(Entity& entity) {
			return m_ComponentRegistry.GetComponent<T>(entity);
		}

		

		private:
			ComponentRegistry m_ComponentRegistry;
	};
}


#endif