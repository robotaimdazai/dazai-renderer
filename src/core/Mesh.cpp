#include "Mesh.hpp"

namespace DazaiEngine
{
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Material& material):
		vertices(vertices), indices(indices), material(material)
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
	auto Mesh::draw(Camera& camera)->void
	{
		material.shader->bind();
		vao.bind();
		
		for (size_t i = 0; i < material.textures.size(); i++)
		{
			material.textures[i].bindToSlot(*material.shader, material.textures[i].texType, material.textures[i].slot);
			material.textures[i].bind();
		}
		material.shader->setVec3("camPos", camera.position);
		camera.bindtoShader(*material.shader, "camMatrix");
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
}