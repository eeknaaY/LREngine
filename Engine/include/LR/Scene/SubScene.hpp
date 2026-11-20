#ifndef LRSUBSCENE_HPP
#define LRSUBSCENE_HPP

#include "Asset.hpp"
#include "GameObject.hpp"

#include <vector>
#include <unordered_map>

namespace LR {
	class SubScene {
	public:
		void AddGameObject(const GameObject& gameobject);
		const std::vector<GameObject>& GetGameObjets() const;

	private:
		std::unordered_map<std::shared_ptr<Shader>, std::vector<GameObject>>m_ShaderObjects;
		std::vector<GameObject> m_Objects;
	};
}

#endif