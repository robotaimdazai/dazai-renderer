#pragma once
#include <glad/glad.h>

namespace DazaiEngine
{
	class FrameBufferTexture2d
	{
	public:
		FrameBufferTexture2d() = delete;
		FrameBufferTexture2d(unsigned int width, unsigned int height, unsigned int colorAttachment);
		auto bind() -> void;
		auto unbind() -> void;
		unsigned int id;
		unsigned int colorAttachment;
		unsigned int width;
		unsigned int height;
	};
}