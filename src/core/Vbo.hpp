#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include <vector>

namespace DazaiEngine 
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec3 tangent;
	};
	class Vbo
	{
		public:
		Vbo() = delete;
		Vbo(std::vector<Vertex>& vertices);
		Vbo(std::vector<glm::mat4>& trsMatrices);
		auto bind() -> void;
		auto unbind() -> void;
		auto destroy() -> void;
		unsigned int id = 0;
	};
}