#pragma once
#include <glad/glad.h>

namespace DazaiEngine 
{
	class Ebo
	{
	public:
		Ebo(GLuint* indices, GLsizeiptr size);
		auto bind() -> void;
		auto unBind() -> void;
		auto destroy() -> void;
		unsigned int id = 0;

	private:
		
	};
}