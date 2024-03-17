#pragma once
#include<iostream>
#include"../util/FileSystem.hpp"
#include "glad/glad.h"

namespace DazaiEngine
{
	static class Util
	{
	public:
		auto static getGlTextureFormatFromExtension(const std::string& extension) -> GLenum
		{
			if (extension == "png")
				return GL_RGBA;
			else if (extension == "jpg")
				return GL_RGB;
		}
	};
}