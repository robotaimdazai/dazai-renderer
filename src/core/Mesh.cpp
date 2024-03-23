#include "Mesh.hpp"

namespace DazaiEngine
{
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material):
		vertices(vertices), indices(indices)
	{
		vao.bind();
		Vbo vbo(vertices);
		Ebo ebo(indices);
		vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
		vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
		vao.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

		vao.unBind();
		vbo.unBind();
		ebo.unBind();
	}
	auto Mesh::draw(Shader& shader, std::vector<Texture2d> textures, Camera& camera)->void
	{
		shader.bind();
		vao.bind();
		
		for (size_t i = 0; i < textures.size(); i++)
		{
			//textures[i].bindToSlot(shader, textures[i].texType, textures[i].slot);
			//textures[i].bind();
		}
		shader.setVec3("camPos", camera.position);
		camera.bindtoShader(shader, "camMatrix");
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
}