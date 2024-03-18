#pragma once
#include <glad/glad.h>

namespace DazaiEngine 
{
	class Vbo
	{
		public:
		Vbo(GLfloat* vertices, GLsizeiptr size);
		auto bind() -> void;
		auto unBind() -> void;
		auto destroy() -> void;
		unsigned int id = 0;
	};
}