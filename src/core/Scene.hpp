#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace DazaiEngine
{
	struct Scene
	{
		glm::vec4 lightColor;
		glm::vec3 lightPos;
		glm::mat4 lightProjection;
		
		inline static const char* LIGHT_COLOR_UNIFORM =			"lightColor";
		inline static const char* LIGHT_POS_UNIFORM =			"lightPos";
		inline static const char* LIGHT_PROJECTION_UNIFORM =	"lightProjection";
		inline static const char* SHADOW_MAP_UNIFORM =			"shadowMap";
	};
}