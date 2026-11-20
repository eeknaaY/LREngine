#include "LR/Scene/SubScene.hpp"

namespace LR {
	void SubScene::AddGameObject(const GameObject& gameobject) {
		m_Objects.push_back(gameobject);
	}

	const std::vector<GameObject>& SubScene::GetGameObjets() const
	{
		return m_Objects;
	}
}
