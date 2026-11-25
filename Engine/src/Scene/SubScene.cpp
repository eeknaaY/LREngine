#include "LR/Scene/SubScene.hpp"

namespace LR {
	SubScene::SubScene() {
		m_Objects.reserve(10);
	}

	void SubScene::AddGameObject(const GameObject& gameobject) {
		m_Objects.push_back(std::make_shared<GameObject>(gameobject));
	}

	std::vector<std::shared_ptr<GameObject>>& SubScene::GetGameObjets()
	{
		return m_Objects;
	}
}
