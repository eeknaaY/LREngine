#ifndef LRSUBSCENE_HPP
#define LRSUBSCENE_HPP

#include "Asset.hpp"
#include "GameObject.hpp"

#include <vector>
#include <unordered_map>

namespace LR {
	class SubScene {
	public:
		SubScene();

		void AddGameObject(const GameObject& gameobject);
		std::vector<std::shared_ptr<GameObject>>& GetGameObjets();

	private:
		std::unordered_map<std::shared_ptr<Shader>, std::vector<GameObject>>m_ShaderObjects;
		std::vector<std::shared_ptr<GameObject>> m_Objects;
	};
}

#endif