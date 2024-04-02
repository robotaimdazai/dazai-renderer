#include "RenderBuffer.hpp"

namespace DazaiEngine
{
	RenderBuffer::RenderBuffer(unsigned int width, unsigned int height):
		width(width), height(height)
	{
		glGenRenderbuffers(1, &id);
		glBindRenderbuffer(GL_RENDERBUFFER, id);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id);
		auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer error: " << fboStatus << std::endl;
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}