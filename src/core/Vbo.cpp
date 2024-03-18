#include "Vbo.hpp"

namespace DazaiEngine
{
	Vbo::Vbo(GLfloat* vertices, GLsizeiptr size)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER,id);
		glBufferData(GL_ARRAY_BUFFER,size,vertices,GL_STATIC_DRAW);
	}
	auto Vbo::bind() -> void
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
	auto Vbo::unBind() -> void
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	auto Vbo::destroy() -> void
	{
		glDeleteBuffers(1, &id);
	}
}