#pragma once
#include "Transform.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
namespace DazaiEngine 
{
	class Light
	{
	public:
		Light();
		auto draw(Camera& camera, Shader& shader) -> void;
		Mesh mesh;
		Transform transform;
		glm::vec4 color;
	};
}