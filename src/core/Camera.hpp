#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "Shader.hpp"

namespace DazaiEngine
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(int width, int height, glm::vec3 position);
		auto setViewProjectionMatrix(float fov, float near, float far,Shader& shader, const char* uniform) -> void;
		auto input(GLFWwindow* window) -> void;

		glm::vec3 mPosition;
		glm::vec3 mForward{ 0.0f,0.0f,-1.0f};
		glm::vec3 mUp{0.0f,1.0f,0.0f};
		int mWidth;
		int mHeight;
		float mSpeed{ 0.1f };
		float mSensitivity{ 100.0f };
	};
}