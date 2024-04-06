#include "Vbo.hpp"

namespace DazaiEngine
{
	Vbo::Vbo(std::vector<Vertex>& vertices)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER,id);
		glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),vertices.data(), GL_STATIC_DRAW);
	}

	Vbo::Vbo(std::vector<glm::mat4>& trsMatrices)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, trsMatrices.size() * sizeof(glm::mat4), trsMatrices.data(), GL_STATIC_DRAW);
	}
	auto Vbo::bind() -> void
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
	auto Vbo::unbind() -> void
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	auto Vbo::destroy() -> void
	{
		glDeleteBuffers(1, &id);
	}
}