#ifndef LRMESH_HPP
#define LRMESH_HPP

#include <vector>
#include <glm/vec3.hpp>

namespace LR {

	struct Vertex {
		glm::vec3 Position;
	};

	// Factory for rendering APIs
	class Mesh {
	public:
		Mesh();
		~Mesh();

		void SetVertices(std::vector<Vertex>& vertices);
		void SetIndicies(std::vector<uint16_t>& vertices);

	private:
		std::vector<Vertex>m_Vertices;
		std::vector<uint16_t>m_Indicies;
	};

}

#endif