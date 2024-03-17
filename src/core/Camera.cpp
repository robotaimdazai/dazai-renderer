#include "Camera.hpp"

namespace DazaiEngine
{
	Camera::Camera(int width, int height, glm::vec3 position):mWidth(width),mHeight(height),mPosition(position)
	{

	}
	auto Camera::setViewProjectionMatrix(float fov, float near, float far, Shader& shader, const char* uniform) -> void
	{
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		view = glm::lookAt(mPosition, mPosition + mForward, mUp);
		proj = glm::perspective(glm::radians(fov), (float)mWidth / mHeight, near, far);
		shader.setMat4(uniform, proj*view );
	}
	auto Camera::input(GLFWwindow* window) -> void
	{

	}
}