#pragma once
#include <glad/glad.h>
#include "Vao.hpp"
#include "Vbo.hpp"
#include <iostream>

namespace DazaiEngine
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		auto createVaoVbo() -> void;
		auto bind() -> void;
		auto unbind() -> void;
		auto bindVao() -> void;
		auto unbindVao() -> void;
		auto setDrawBuffer(GLenum drawBuffer) -> void;
		auto setReadBuffer(GLenum readBuffer) -> void;
		unsigned int id;
		unsigned int vao;
		unsigned int vbo;
	};
}