#pragma once
#include "Vbo.hpp"
#include <glad/glad.h>

namespace DazaiEngine
{
	class Vao
	{
	public:
		Vao();

		auto linkVbo(Vbo& vbo, GLuint layout) -> void;
		auto id() -> unsigned int;
		auto bind() -> void;
		auto unBind() -> void;
		auto destroy() -> void;
	private:
		unsigned int mId;
	};
}