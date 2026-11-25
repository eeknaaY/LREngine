#ifndef LRMESH_HPP
#define LRMESH_HPP

#include <vector>

#include "Interfaces/GenericMesh.hpp"
#include "../Core/Core.hpp"
#include "Vulkan/VulkanMesh.hpp"

namespace LR {

	class Mesh {
	public:
		Mesh();
		Mesh(const std::vector<Vertex>& Vertices, const std::vector<uint16_t>& Indicies);
		Mesh(const std::vector<Vertex>& Vertices, const std::vector<uint16_t>& Indicies, VulkanAPI* api);
		~Mesh();

		template <typename T> requires std::derived_from<T, GenericMesh>
		T* Get() { return static_cast<T*>(m_Mesh); }

		void Bind();

		void SetVertices(const std::vector<Vertex>& vertices);
		void SetIndicies(const std::vector<uint16_t>& indicies);
		std::vector<Vertex> GetVertices();
		std::vector<uint16_t> GetIndicies();

	private:
		GenericMesh* m_Mesh = nullptr;
	};

}

#endif