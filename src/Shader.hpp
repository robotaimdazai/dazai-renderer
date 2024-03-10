#pragma once
#include <iostream>
#include <glad/glad.h>

namespace DazaiEngine 
{
	class Shader
	{

	public:
		Shader() = default;
		Shader(const std::string& path, unsigned int shaderType);
		auto id() -> unsigned int;
		auto destroy()->void;

	private:
		unsigned int m_shader_id;
	};
}

