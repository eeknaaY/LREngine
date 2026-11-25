#include "LR/Graphics/Mesh.hpp"

namespace LR {
	Mesh::Mesh() {

	}

	Mesh::Mesh(const std::vector<Vertex>& Vertices, const std::vector<uint16_t>& Indicies) {
		Core::API_TYPE API = Core::settings.GetAPIUsage();
		
		switch (API) {
		case Core::API_TYPE::VULKAN:
			throw std::runtime_error("Vulkan shaders require the Renderer to be passed with.");
		default:
			throw std::runtime_error("Invalid/Unsupported graphics API selected for LRMesh.");
		}
	}

	Mesh::Mesh(const std::vector<Vertex>& Vertices, const std::vector<uint16_t>& Indicies, VulkanAPI* api) {
		Core::API_TYPE API = Core::settings.GetAPIUsage();

		switch (API) {
		case Core::API_TYPE::VULKAN:
			m_Mesh = new VulkanMesh(api, Vertices, Indicies);
			break;
		default:
			throw std::runtime_error("Invalid/Unsupported graphics API selected for LRMesh.");
		}
	}

	Mesh::~Mesh() {

	}

	void Mesh::Bind() {
		if (!m_Mesh) return;
		m_Mesh->Bind();
	}

	void Mesh::SetVertices(const std::vector<Vertex>& vertices) {
		if (!m_Mesh) return;
		m_Mesh->m_Vertices = std::move(vertices);
	}

	void Mesh::SetIndicies(const std::vector<uint16_t>& indicies) {
		if (!m_Mesh) return;
		m_Mesh->m_Indicies = std::move(indicies);
	}

	std::vector<Vertex> Mesh::GetVertices()
	{
		if (!m_Mesh) return std::vector<Vertex>();
		return m_Mesh->m_Vertices;
	}

	std::vector<uint16_t> Mesh::GetIndicies()
	{
		if (!m_Mesh) return std::vector<uint16_t>();
		return m_Mesh->m_Indicies;
	}
}