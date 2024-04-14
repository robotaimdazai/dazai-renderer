#include "FrameBufferTexture2d.hpp"
#include "FrameBufferTexture2d.hpp"

namespace DazaiEngine
{
	FrameBufferTexture2d::FrameBufferTexture2d(unsigned int width, unsigned int height, 
		GLenum attachment, GLenum internalFormat, GLenum format, GLenum datatype, bool clampColorToEdge):
		width(width),height(height), attachment(attachment)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, datatype, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		if (clampColorToEdge)
		{
			float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id,0);
		
	}
	auto FrameBufferTexture2d::bind() -> void
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}
	
	auto FrameBufferTexture2d::unbind() -> void
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}