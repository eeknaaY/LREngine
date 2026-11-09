#ifndef LRRENDERER_HPP
#define LRRENDERER_HPP

#include "LRVulkanAPI.hpp"

// Factory for rendering APIs
class LRRenderer {
public:
	LRRenderer(LR::API_TYPE Type);
	~LRRenderer();

private:
	LRGraphicsAPI* m_GraphicsAPI = nullptr;

	bool Initialize();
};

#endif