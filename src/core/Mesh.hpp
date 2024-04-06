#pragma once 
#include<string>
#include "Vbo.hpp"
#include "Vao.hpp"
#include "Ebo.hpp"
#include "Camera.hpp"
#include "Texture2d.hpp"
#include "Material.hpp"
#include "Scene.hpp"

namespace DazaiEngine 
{
	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices,
			unsigned int instances = 1, std::vector<glm::mat4> instanceMatrix = {});
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		auto draw(Shader& shader, std::vector<Texture2d> textures, Camera& camera, const Scene& scene, 
			glm::vec3& position, glm::quat& rotation, glm::vec3& scale)->void;
		Vao vao;
		unsigned int instances;
	};
}