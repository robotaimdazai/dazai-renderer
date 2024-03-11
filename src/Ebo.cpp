#include "Ebo.hpp"

namespace DazaiEngine
{
	Ebo::Ebo(GLuint* indices, GLsizeiptr size)
	{
		glGenBuffers(1, &mId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,indices,GL_STATIC_DRAW);
	}
	auto Ebo::id() -> unsigned int
	{
		return mId;
	}
	auto Ebo::bind() -> void
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
	}
	auto Ebo::unBind() -> void
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	auto Ebo::destroy() -> void
	{
		glDeleteBuffers(1, &mId);
	}
}