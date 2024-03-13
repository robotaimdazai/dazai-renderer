#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glad/glad.h>
#include "util/FileSystem.hpp"
#include <iostream>


namespace DazaiEngine 
{
	class Shader
	{

	public:
		Shader() = default;
		Shader(const std::string& vertPath, const std::string& fragPath);
		~Shader();
		auto bind() -> void;
		auto unbind()->void;
		auto destroy()->void;
		auto id() -> unsigned int;
		auto setFloat(const char* name, GLfloat value) -> void;

	private:
		unsigned int mId = 0;
	};
}

