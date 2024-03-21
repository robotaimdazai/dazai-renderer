#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>

namespace DazaiEngine 
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 uv;
	};
	class Vbo
	{
		public:
		Vbo(std::vector<Vertex> vertices);
		auto bind() -> void;
		auto unBind() -> void;
		auto destroy() -> void;
		unsigned int id = 0;
	};
}