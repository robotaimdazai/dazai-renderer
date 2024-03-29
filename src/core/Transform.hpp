#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace DazaiEngine
{
	struct Transform
	{
		glm::vec3 position{0,0,0};
		glm::quat rotation{ 0,0,0,0 };
		glm::vec3 scale{1.0f,1.0f,1.0f};
	};
}