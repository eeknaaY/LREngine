#ifndef LRGAMEOBJECT_HPP
#define LRGAMEOBJECT_HPP

#include "../Graphics/Mesh.hpp"
#include "../Graphics/Shader.hpp"

namespace LR {
	class GameObject {
	public:
		GameObject();
		~GameObject();

		void SetShader(const std::shared_ptr<Shader>& shader);
		void SetMesh(const std::shared_ptr<Mesh>& shader);

		std::shared_ptr<Shader> GetShader() const;
		std::shared_ptr<Mesh> GetMesh();

	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Shader> m_Shader;
	};
}

#endif