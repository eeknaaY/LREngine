#ifndef LRVULKANAPI_HPP
#define LRVULKANAPI_HPP

#include "LRGraphicsAPI.hpp"

class LRVulkanAPI : public LRGraphicsAPI {
public:
	void Draw() override;
	bool Initialize() override;
};

#endif