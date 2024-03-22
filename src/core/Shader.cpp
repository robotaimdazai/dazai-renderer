#include "shader.hpp"


namespace DazaiEngine 
{
	Shader::Shader(const std::string& vertPath, const std::string& fragPath)
	{
		auto vertId = glCreateShader(GL_VERTEX_SHADER);
		auto fragId = glCreateShader(GL_FRAGMENT_SHADER);
		const std::string  vertSrc = Resources::readTextFile(vertPath);
		const std::string  fragSrc = Resources::readTextFile(fragPath);
		const char* vertSrcPtr = vertSrc.c_str();
		const char* fragSrcPtr = fragSrc.c_str();
		if (vertSrcPtr == "" || fragSrcPtr == "")
		{
			std::cout << "Shader:: Shader source is null" << std::endl;
		}
		else 
		{
			glShaderSource(vertId, 1, &vertSrcPtr, NULL);
			glShaderSource(fragId, 1, &fragSrcPtr, NULL);
			glCompileShader(vertId);
			glCompileShader(fragId);

			GLuint shaderProgram = glCreateProgram();
			id = shaderProgram;
			glAttachShader(shaderProgram, vertId);
			glAttachShader(shaderProgram, fragId);
			glLinkProgram(shaderProgram);

			GLint linkSuccess;
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
			if (!linkSuccess)
			{
				GLchar infoLog[512];
				glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
				std::cout << vertPath << std::endl;
				std::cout << "Shader program linking error: " << infoLog << std::endl;
				
			}
			glDeleteShader(vertId);
			glDeleteShader(fragId);
		}
	}
	Shader::~Shader()
	{
		destroy();
	}
	auto Shader::destroy() -> void
	{
		glDeleteShader(id);
	}
	auto Shader::setFloat(const char* name, GLfloat value) -> void
	{
		GLuint uniformId = glGetUniformLocation(id,name);
		glUniform1f(uniformId,value);
	}
	auto Shader::setInt(const char* name, GLint value) -> void
	{
		GLuint uniformId = glGetUniformLocation(id, name);
		glUniform1i(uniformId, value);
	}
	auto Shader::setMat4(const char* name, glm::mat4 value) -> void
	{
		GLuint uniformId = glGetUniformLocation(id, name);
		glUniformMatrix4fv(uniformId,1,GL_FALSE,glm::value_ptr(value));
	}
	auto Shader::setVec4(const char* name, glm::vec4 value) -> void
	{
		GLuint uniformId = glGetUniformLocation(id, name);
		glUniform4f(uniformId, value.x,value.y,value.z,value.w);
	}
	auto Shader::setVec3(const char* name, glm::vec3 value) -> void
	{
		GLuint uniformId = glGetUniformLocation(id, name);
		glUniform3f(uniformId, value.x, value.y, value.z);
	}
	auto Shader::bind() -> void
	{
		glUseProgram(id);
	}
	auto Shader::unbind() -> void
	{
		glUseProgram(0);
	}

}