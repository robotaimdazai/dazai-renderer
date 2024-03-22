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
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		Vao vao;
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material);
		auto draw(Camera& camera)->void;
		Material material;
	};
}