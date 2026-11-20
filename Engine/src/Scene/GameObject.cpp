#include "LR/Scene/GameObject.hpp"

namespace LR {
	GameObject::GameObject() {

	}

	GameObject::~GameObject() {

	}

	void GameObject::SetShader(const std::shared_ptr<Shader>& shader) {
		m_Shader = shader;
	}

	void GameObject::SetMesh(const Mesh& mesh) {
		m_Mesh = mesh;
	}

	const std::shared_ptr<Shader> GameObject::GetShader() const
	{
		return m_Shader;
	}

	const Mesh& GameObject::GetMesh() const
	{
		return m_Mesh;
	}
}
