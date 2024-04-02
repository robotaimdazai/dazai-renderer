#pragma once
#include <glad/glad.h>
#include <iostream>

namespace DazaiEngine
{
	class RenderBuffer
	{
	public:
		RenderBuffer() = delete;
		RenderBuffer(unsigned int width, unsigned int height);
		unsigned int width;
		unsigned int height;
		unsigned int id;
	};
}