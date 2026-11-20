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
		void SetMesh(const Mesh& shader);

		const std::shared_ptr<Shader> GetShader() const;
		const Mesh& GetMesh() const;

	private:
		Mesh m_Mesh;
		std::shared_ptr<Shader> m_Shader;
	};
}

#endif