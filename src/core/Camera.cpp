#include "Camera.hpp"
#include "Camera.hpp"

namespace DazaiEngine
{
	Camera::Camera(int width, int height, glm::vec3 position):width(width), height(height), position(position)
	{

	}
	auto Camera::updateMatrix(float fov, float near, float far) -> void
	{
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
		view = glm::lookAt(position, position + forward, up);
		proj = glm::perspective(glm::radians(fov), (float)width / height, near, far);
		mMatrix = proj * view;
	}
	auto Camera::bindtoShader(Shader& shader, const char* uniform) -> void
	{
		shader.setMat4(uniform, mMatrix);
	}
	auto Camera::input(GLFWwindow* window) -> void
	{
		//keyboard
		currentSpeed *= Time::deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			position += currentSpeed * forward;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			position += currentSpeed * - glm::normalize(glm::cross(forward, up));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			position += currentSpeed * -forward;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			position += currentSpeed * glm::normalize(glm::cross(forward, up));
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			position += currentSpeed * up;
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			position += currentSpeed * -up;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			currentSpeed = maxSpeed;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		{
			currentSpeed = normalSpeed;
		}
		//mouse
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			if (mFirstClick)
			{
				glfwSetCursorPos(window, (width / 2), (height / 2));
				mFirstClick = false;
			}
			double mouseX;
			double mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);
			float rotX = Time::deltaTime * sensitivity * (float)(mouseY - (height / 2)) / height;
			float rotY = Time::deltaTime * sensitivity * (float)(mouseX - (width / 2)) / width;
			glm::vec3 newForward = glm::rotate(forward, glm::radians(-rotX), glm::normalize(glm::cross(forward, up)));
			if (abs(glm::angle(newForward, up) - glm::radians(90.0f) <= glm::radians(85.0f)))
			{
				forward = newForward;
			}
			forward = glm::rotate(forward, glm::radians(-rotY), up);
			glfwSetCursorPos(window, (width / 2), (height / 2));
			
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			mFirstClick = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}