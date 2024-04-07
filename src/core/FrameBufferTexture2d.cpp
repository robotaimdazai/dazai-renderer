#include "FrameBufferTexture2d.hpp"
#include "FrameBufferTexture2d.hpp"

namespace DazaiEngine
{
	FrameBufferTexture2d::FrameBufferTexture2d(unsigned int width, unsigned int height, 
		GLenum attachment, GLenum internalFormat, GLenum format, GLenum datatype, unsigned int slot):
		width(width),height(height), attachment(attachment), slot(slot)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, datatype, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id, 0);
		
	}
	auto FrameBufferTexture2d::bind() -> void
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	auto FrameBufferTexture2d::bindToSlot(Shader& shader, const char* uniform) -> void
	{
		shader.bind();
		shader.setInt(uniform, GL_TEXTURE0 + slot);
	}
	auto FrameBufferTexture2d::unbind() -> void
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}