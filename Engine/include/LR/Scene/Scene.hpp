#ifndef LRSCENE_HPP
#define LRSCENE_HPP

#include <vector>
#include <memory>

#include "SubScene.hpp"

namespace LR {
	class Scene {
	public:
		std::shared_ptr<SubScene> GetSubscene();

	private:
		std::vector<std::shared_ptr<SubScene>>m_SubScenes;
	};
}

#endif