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
		auto setDrawBuffer(const unsigned int count,GLenum drawBuffers[]) -> void;
		auto setReadBuffer(GLenum readBuffers) -> void;
		unsigned int id;
		unsigned int vao;
		unsigned int vbo;
	};
}