#include "FrameBuffer.hpp"
#include "FrameBuffer.hpp"
#include "FrameBuffer.hpp"
#include "FrameBuffer.hpp"

namespace DazaiEngine
{
	float rectangleVertices[] =
	{
		// Coords    // texCoords
		-1.0f, -1.0f,  0.0f, 0.0f, 
		1.0f, -1.0f,  1.0f, 0.0f,  
		1.0f,  1.0f,  1.0f, 1.0f,  

		-1.0f,  1.0f,  0.0f, 1.0f, 
		-1.0f, -1.0f,  0.0f, 0.0f, 
		1.0f,  1.0f,  1.0f, 1.0f   
	};

	FrameBuffer::FrameBuffer()
	{
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}
	auto FrameBuffer::createVaoVbo() -> void
	{
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	auto FrameBuffer::bind() -> void
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}
	auto FrameBuffer::unbind() -> void
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	auto FrameBuffer::bindVao() -> void
	{
		glBindVertexArray(vao);
	}
	auto FrameBuffer::unbindVao() -> void
	{
		glBindVertexArray(0);
	}
	auto FrameBuffer::setDrawBuffer(GLenum drawBuffer) -> void
	{
		glDrawBuffer(drawBuffer);
	}
	auto FrameBuffer::setReadBuffer(GLenum readBuffer) -> void
	{
		glReadBuffer(readBuffer);
	}
}