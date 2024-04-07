#pragma once
#include<glad/glad.h>
#include <iostream>
#include "../util/Resources.hpp"
#include "Shader.hpp"

namespace DazaiEngine
{
	class TextureCubemap
	{
	public:
		TextureCubemap() = delete;
		TextureCubemap
		(
			const std::string& right,
			const std::string& left,
			const std::string& top,
			const std::string& bottom,
			const std::string& front,
			const std::string& back
		);
		auto bind() -> void;
		auto bindToSlot(Shader& shader, const char* uniform) -> void;
		auto unbind() -> void;

		 int width;
		 int height;
		 unsigned int id;
		 int numChannels;
	};
}