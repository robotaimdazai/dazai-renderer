#pragma once
#include <glad/glad.h>

namespace DazaiEngine 
{
	class Ebo
	{
	public:
		Ebo(GLuint* indices, GLsizeiptr size);
		auto id() -> unsigned int;
		auto bind() -> void;
		auto unBind() -> void;
		auto destroy() -> void;

	private:
		unsigned int mId = 0;
	};
}