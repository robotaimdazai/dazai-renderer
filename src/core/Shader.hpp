#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glad/glad.h>
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
		Shader(const std::string& vertPath, const std::string& fragPath, const std::string& geometryShader="");
		~Shader();
		auto bind() -> void;
		auto unbind()->void;
		auto destroy()->void;
		auto setFloat(const char* name, GLfloat value) -> void;
		auto setInt(const char* name, GLint value) -> void;
		auto setMat4(const char* name, glm::mat4 value) -> void;
		auto setVec4(const char* name, glm::vec4 value) -> void;
		auto setVec3(const char* name, glm::vec3 value) -> void;
		unsigned int id = 0;
	};
}

