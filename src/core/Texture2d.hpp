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
		auto id() -> unsigned int;
		auto width() ->  int;
		auto height() ->  int;
		auto numChannels() ->  int;
		auto type() -> GLenum;
		auto slot() -> unsigned int;

	private:
		unsigned int mId;
		int mWidth;
		int mHeight;
		int mNumChannels;
		unsigned int mSlot;
		GLenum mType;
	};
}