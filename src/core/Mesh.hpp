#pragma once 
#include<string>
#include "Vbo.hpp"
#include "Vao.hpp"
#include "Ebo.hpp"
#include "Camera.hpp"
#include "Texture2d.hpp"
#include "Material.hpp"

namespace DazaiEngine 
{
	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material);
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		Vao vao;
		auto draw(Shader& shader, std::vector<Texture2d> textures ,Camera& camera)->void;
	};
}