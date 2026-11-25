#ifndef LRGENERICMESH_HPP
#define LRGENERICMESH_HPP

#include <vector>
#include <glm/vec3.hpp>

namespace LR {

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Color;
	};

	class GenericMesh {
	public:
		virtual void Bind() = 0;

		// Public since generic mesh is never supposed to be used by itself, it is just supposed to exist in Mesh.
		std::vector<Vertex>m_Vertices;
		std::vector<uint16_t>m_Indicies;

	};
}

#endif