#ifndef LRTRANSFORMCOMPONENT_HPP
#define LRTRANSFORMCOMPONENT_HPP

#include "Component.hpp"

#include <glm/vec3.hpp>

namespace LR::Components {

	class Transform : public Component {
	public:
		glm::vec3 Position;

	};
}


#endif