#pragma once
#include <glad/glad.h>
#include "../../include/stb/stb_image.h"
#include "Shader.hpp"

namespace DazaiEngine
{
	class Texture2d
	{
	public:
		Texture2d() = default;
		Texture2d(const std::string& path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
		auto bindToShader(Shader& shader, const char* uniform, GLuint slot) -> void;
		auto bind() -> void;
		auto unbind() -> void;
		auto destroy() -> void;
		unsigned int id;
		int width;
		int height;
		int mNumChannels;
		unsigned int slot;
		GLenum texType;
		
	};
}