#pragma once 
#include<string>
#include "Vbo.hpp"
#include "Vao.hpp"
#include "Ebo.hpp"
#include "Camera.hpp"
#include "Texture2d.hpp"

namespace DazaiEngine 
{
	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture2d> textures;
		Vao vao;
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture2d>& textures);
		auto draw(Shader& shader, Camera& camera)->void;
	};
}