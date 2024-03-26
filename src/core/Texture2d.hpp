#pragma once
#include <glad/glad.h>
#include "stb/stb_image.h"
#include "Shader.hpp"

namespace DazaiEngine
{
	class Texture2d
	{
	public:
		Texture2d() = default;
		Texture2d(const std::string& path, const char* texType, unsigned int slot, GLenum format, GLenum pixelType);
		Texture2d( unsigned char* bytes,int width, int height, int numChannels, const char* texType, unsigned int slot, GLenum format, GLenum pixelType);
		auto bindToSlot(Shader& shader, const char* uniform, unsigned int slot) -> void;
		auto bind() -> void;
		auto unbind() -> void;
		auto destroy() -> void;
		unsigned int id;
		int width;
		int height;
		int numChannels;
		unsigned int slot;
		const char* texType;
	};
}