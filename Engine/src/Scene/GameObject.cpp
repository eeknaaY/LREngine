#include "LR/Scene/GameObject.hpp"

namespace LR {
	GameObject::GameObject() {

	}

	GameObject::~GameObject() {

	}

	void GameObject::SetShader(const std::shared_ptr<Shader>& shader) {
		m_Shader = shader;
	}

	void GameObject::SetMesh(const std::shared_ptr<Mesh>& mesh) {
		m_Mesh = mesh;
	}

	std::shared_ptr<Shader> GameObject::GetShader() const
	{
		return m_Shader;
	}

	std::shared_ptr<Mesh> GameObject::GetMesh()
	{
		return m_Mesh;
	}
}
