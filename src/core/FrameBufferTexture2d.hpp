#pragma once
#include <glad/glad.h>
#include "Shader.hpp"

namespace DazaiEngine
{
	class FrameBufferTexture2d
	{
	public:
		FrameBufferTexture2d() = delete;
		FrameBufferTexture2d(unsigned int width, unsigned int height, GLenum attachment,
			GLenum internalFormat, GLenum format, GLenum datatype, unsigned int slot);
		auto bind() -> void;
		auto bindToSlot(Shader& shader, const char* uniform) -> void;
		auto unbind() -> void;
		unsigned int id;
		GLenum attachment;
		unsigned int width;
		unsigned int height;
		unsigned int slot;
	};
}