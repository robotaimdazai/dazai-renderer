#include "Vao.hpp"

namespace DazaiEngine
{
	Vao::Vao()
	{
		glGenVertexArrays(1,&mId);
		
	}
	auto Vao::linkVbo(Vbo& vbo, GLuint layout) -> void
	{
		vbo.bind();
		glVertexAttribPointer(layout,3,GL_FLOAT,GL_FALSE,0,(void*)0);
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