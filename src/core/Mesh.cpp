#include "Mesh.hpp"

namespace DazaiEngine
{
	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, 
		unsigned int instances, std::vector<glm::mat4> instanceMatrix):
		vertices(vertices), indices(indices), instances(instances)
	{
		vao.bind();
		Vbo instanceVbo(instanceMatrix);
		Vbo vbo(vertices);
		Ebo ebo(indices);
		vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
		vao.linkAttrib(vbo, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
		vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));
		vao.linkAttrib(vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));
		if (instances != 1)
		{
			instanceVbo.bind();
			vao.linkAttrib(instanceVbo, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
			vao.linkAttrib(instanceVbo, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void*) sizeof(glm::vec4));
			vao.linkAttrib(instanceVbo, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*) (2 * sizeof(glm::vec4)));
			vao.linkAttrib(instanceVbo, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void*) (3 * sizeof(glm::vec4)));
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
		}
		instanceVbo.unbind();
		vao.unbind();
		vbo.unbind();
		ebo.unbind();
	}
	auto Mesh::draw(Shader& shader, std::vector<Texture2d> textures, Camera& camera, const Scene& scene, 
		glm::vec3& position, glm::quat& rotation, glm::vec3& scale
		)->void
	{
		vao.bind();
		shader.bind();
		shader.setVec3("camPos", camera.position);
		camera.bindtoShader(shader, "camMatrix");
		shader.setVec4(Scene::LIGHT_COLOR_UNIFORM, scene.lightColor);
		shader.setVec3(Scene::LIGHT_POS_UNIFORM, scene.lightPos);
		shader.setMat4(Scene::LIGHT_PROJECTION_UNIFORM, scene.lightProjection);
		for (size_t i = 0; i < textures.size(); i++)
		{
			textures[i].bind();
			textures[i].bindToSlot(shader, textures[i].texType);
		}
		if (instances == 1)
		{
			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);
			trans = glm::translate(trans, position);
			rot = glm::mat4_cast(rotation);
			sca = glm::scale(sca, scale);
			shader.setMat4("translation", trans);
			shader.setMat4("rotation", rot);
			shader.setMat4("scale", sca);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0,instances);
		}
		
	}
}