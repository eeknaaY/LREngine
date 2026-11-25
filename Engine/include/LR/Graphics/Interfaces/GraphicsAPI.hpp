#ifndef LRGRAPHICSAPI_HPP
#define LRGRAPHICSAPI_HPP

#include "../../Scene/SubScene.hpp"

namespace LR {

class GraphicsAPI {
public:
	GraphicsAPI() = default;

	virtual bool Initialize() = 0;
	virtual void Draw(SubScene& subscene) = 0;
};

}

#endif