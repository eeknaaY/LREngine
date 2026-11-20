#include "LR/Graphics/Mesh.hpp"

namespace LR {
	Mesh::Mesh() {

	}

	Mesh::~Mesh() {

	}
	void Mesh::SetVertices(std::vector<Vertex>& vertices) {
		m_Vertices = std::move(vertices);
	}

	void Mesh::SetIndicies(std::vector<uint16_t>& indicies) {
		m_Indicies = std::move(indicies);
	}
}