#ifndef LRGRAPHICSAPI_HPP
#define LRGRAPHICSAPI_HPP

namespace LR {

class GraphicsAPI {
public:
	GraphicsAPI() = default;

	virtual bool Initialize() = 0;
	virtual void Draw() = 0;
};

}

#endif