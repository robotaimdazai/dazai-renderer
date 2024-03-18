#include "Ebo.hpp"

namespace DazaiEngine
{
	Ebo::Ebo(GLuint* indices, GLsizeiptr size)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,indices,GL_STATIC_DRAW);
	}
	auto Ebo::bind() -> void
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
	auto Ebo::unBind() -> void
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	auto Ebo::destroy() -> void
	{
		glDeleteBuffers(1, &id);
	}
}