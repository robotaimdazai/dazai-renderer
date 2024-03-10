#include "shader.hpp"
#include <glad/glad.h>
#include "util/FileSystem.hpp"
#include <iostream>


namespace DazaiEngine 
{
	Shader::Shader(const std::string& path, unsigned int shaderType)
	{
		m_shader_id = glCreateShader(shaderType);
		const std::string src = FileSystem::loadFile(path);
		const char* srcPtr = src.c_str();
		if (src == "") 
		{
			std::cout << "Shader:: Shader source is null" << std::endl;
		}
		else 
		{
			glShaderSource(m_shader_id, 1, &srcPtr, NULL);
			glCompileShader(m_shader_id);
		}
	}

	auto Shader::id()->unsigned int
	{
		return m_shader_id;
	}

	auto Shader::destroy() -> void
	{
		glDeleteShader(m_shader_id);
	}

}