#include "Mesh.hpp"

namespace DazaiEngine
{
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices):
		vertices(vertices), indices(indices)
	{
		vao.bind();
		Vbo vbo(vertices);
		Ebo ebo(indices);
		vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		vao.linkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
		vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));
		vao.linkAttrib(vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));
		vao.unBind();
		vbo.unBind();
		ebo.unBind();
	}
	auto Mesh::draw(Shader& shader, std::vector<Texture2d> textures, Camera& camera, const Scene& scene, 
		glm::vec3& position, glm::quat& rotation, glm::vec3& scale
		)->void
	{
		//create modelMatrix
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);
		
		trans = glm::translate(trans, position);
		rot = glm::mat4_cast(rotation);
		sca = glm::scale(sca, scale);
		//bind to shader
		shader.bind();
		shader.setVec4(Scene::LIGHT_COLOR_UNIFORM,scene.lightColor);
		shader.setVec3(Scene::LIGHT_POS_UNIFORM,scene.lightPos);
		shader.setMat4("translation", trans);
		shader.setMat4("rotation", rot);
		shader.setMat4("scale", sca);
		
		vao.bind();
		
		for (size_t i = 0; i < textures.size(); i++)
		{
			textures[i].bindToSlot(shader, textures[i].texType, textures[i].slot);
			textures[i].bind();
		}
		shader.setVec3("camPos", camera.position);
		camera.bindtoShader(shader, "camMatrix");
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		for (size_t i = 0; i < textures.size(); i++)
		{
			textures[i].unbind();
		}
	}
}