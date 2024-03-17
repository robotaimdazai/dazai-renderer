#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glad/glad.h>
#include "../util/FileSystem.hpp";
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
		auto setInt(const char* name, GLint value) -> void;
		auto setMat4(const char* name, glm::mat4 value) -> void;

	private:
		unsigned int mId = 0;
	};
}

