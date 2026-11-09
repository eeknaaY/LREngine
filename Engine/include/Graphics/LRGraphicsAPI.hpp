#ifndef LRGRAPHICSAPI_HPP
#define LRGRAPHICSAPI_HPP

namespace LR {
	enum class API_TYPE {
		VULKAN
	};
}

class LRGraphicsAPI {
public:
	LRGraphicsAPI();
	~LRGraphicsAPI();

	virtual bool Initialize() = 0;
	virtual void Draw() = 0;
};

#endif