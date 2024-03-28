#pragma once
#include "Vbo.hpp"
#include <glad/glad.h>

namespace DazaiEngine
{
	class Vao
	{
	public:
		Vao();
		auto linkAttrib(Vbo& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) -> void;
		auto bind() -> void;
		auto unBind() -> void;
		auto destroy() -> void;
		unsigned int id;
		
	};
}