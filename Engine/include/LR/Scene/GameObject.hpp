#ifndef LRGAMEOBJECT_HPP
#define LRGAMEOBJECT_HPP

#include "../Graphics/Mesh.hpp"
#include "../Graphics/Shader.hpp"
#include "../ECS/EntityManager.hpp"

namespace LR {
	class GameObject {
	public:
		GameObject();
		~GameObject();

		void SetShader(const std::shared_ptr<Shader>& shader);
		void SetMesh(const std::shared_ptr<Mesh>& shader);

		template <typename T> requires std::derived_from<T, Components::Component>
		std::shared_ptr<T> AddComponent() {
			return m_EntityManager.AddComponent<T>(m_Entity);
		}

		template <typename T> requires std::derived_from<T, Components::Component>
		std::shared_ptr<T> GetComponent() {
			return m_EntityManager.GetComponent<T>(m_Entity);
		}

		std::shared_ptr<Shader> GetShader();
		std::shared_ptr<Mesh> GetMesh();

	private:
		inline static EntityManager m_EntityManager;
		Entity m_Entity;

		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Shader> m_Shader;
	};
}

#endif