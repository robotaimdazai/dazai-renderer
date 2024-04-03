#pragma once
#include<glad/glad.h>
#include <iostream>
#include "../util/Resources.hpp"

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
		auto unbind() -> void;

		 int width;
		 int height;
		 unsigned int id;
		 int numChannels;
	};
}