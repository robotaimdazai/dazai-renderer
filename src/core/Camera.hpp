#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "Shader.hpp"
#include "Time.hpp"

namespace DazaiEngine
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(int width, int height, glm::vec3 position);
		auto updateMatrix(float fov, float near, float far) -> void;
		auto bindtoShader(Shader& shader, const char* uniform) -> void;
		auto input(GLFWwindow* window) -> void;

		glm::vec3 position;
		glm::vec3 forward{ 0.0f,0.0f,-1.0f};
		glm::vec3 up{0.0f,1.0f,0.0f};
		int width;
		int height;
		float speed{ 0.7f };
		float sensitivity{ 10000.0f };

	private:
		bool mFirstClick{true};
		glm::mat4 mMatrix;

	};
}