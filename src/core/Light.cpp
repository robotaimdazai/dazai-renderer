#include "Light.hpp"
#include "Vbo.hpp"


namespace DazaiEngine
{
	Light::Light()
	{
		DazaiEngine::Vertex lightVertices[] =
		{ //     COORDINATES     //
			Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
			Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
			Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
			Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
			Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
			Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
			Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
			Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
		};
		GLuint lightIndices[] =
		{
			0, 1, 2,
			0, 2, 3,
			0, 4, 7,
			0, 7, 3,
			3, 7, 6,
			3, 6, 2,
			2, 6, 5,
			2, 5, 1,
			1, 5, 4,
			1, 4, 0,
			4, 5, 6,
			4, 6, 7
		};
		std::vector<DazaiEngine::Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(DazaiEngine::Vertex));
		std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
		mesh = Mesh(lightVerts, lightInd);
		color = glm::vec4(1);
	}
	auto Light::draw(Camera& camera, Shader& shader) -> void
	{
		shader.bind();
		shader.setVec4("lightColor", color);
		mesh.draw(shader, camera,transform.position,transform.rotation, transform.scale);
	}
}