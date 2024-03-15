#include "Vao.hpp"
#include "Vao.hpp"

namespace DazaiEngine
{
	Vao::Vao()
	{
		glGenVertexArrays(1,&mId);
		
	}
	
	auto Vao::linkAttrib(Vbo& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) -> void
	{

		vbo.bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		vbo.unBind();

	}
	auto Vao::id() -> unsigned int
	{
		return mId;
	}
	auto Vao::bind() -> void
	{
		glBindVertexArray(mId);
	}
	auto Vao::unBind() -> void
	{
		glBindVertexArray(0);
	}
	auto Vao::destroy() -> void
	{
		glDeleteVertexArrays(1, &mId);
	}
}