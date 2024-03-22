// DazaiRenderer.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "core/Shader.hpp"
#include "core/Vao.hpp"
#include "core/Vbo.hpp"
#include "core/Ebo.hpp"
#include "stb/stb_image.h"
#include "core/Texture2d.hpp"
#include "util/Util.hpp"
#include "core/Camera.hpp"
#include "core/Time.hpp"
#include "core/Mesh.hpp"
#include "core/Material.hpp"


const unsigned int width = 800;
const unsigned int height = 600;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800,600, "Dazai",NULL,NULL);
	
	if (window == NULL) {
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glEnable(GL_DEPTH_TEST);
	glViewport(0,0,width,height); 

	// Vertices coordinates
	DazaiEngine::Vertex vertices[] =
	{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
		DazaiEngine::Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
		DazaiEngine::Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
		DazaiEngine::Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
		DazaiEngine::Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
	};


	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	DazaiEngine::Vertex lightVertices[] =
	{ //     COORDINATES     //
		DazaiEngine::Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
		DazaiEngine::Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
		DazaiEngine::Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
		DazaiEngine::Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
		DazaiEngine::Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
		DazaiEngine::Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
		DazaiEngine::Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
		DazaiEngine::Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
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

	//shaders
	DazaiEngine::Shader shader("shaders/default.vert", "shaders/default.frag");
	DazaiEngine::Shader lightShader("shaders/light.vert", "shaders/light.frag");
	//camera
	DazaiEngine::Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	//textures
	DazaiEngine::Texture2d textures[]
	{
		DazaiEngine::Texture2d("textures/planks.png","diffuse0", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		DazaiEngine::Texture2d("textures/planksSpec.png","specular0", 1, GL_RED, GL_UNSIGNED_BYTE)
	};
	//meshes
	//floor
	std::vector<DazaiEngine::Vertex> vert(vertices, vertices + sizeof(vertices) / sizeof(DazaiEngine::Vertex));
	std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<DazaiEngine::Texture2d> tex(textures, textures + sizeof(textures) / sizeof(DazaiEngine::Texture2d));
	//light
	std::vector<DazaiEngine::Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(DazaiEngine::Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	

	glm::mat4 lightModel = glm::mat4(1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec4 lightColor = { 1.0f,1.0f,1.0f,1.0f };
	lightModel = glm::translate(lightModel, lightPos);
	DazaiEngine::Material lightMat(&lightShader, tex);
	lightMat.shader->bind();
	lightMat.shader->setMat4("model", lightModel);
	lightMat.shader->setVec4("lightColor", lightColor);

	
	glm::mat4 objectModel = glm::mat4(1.0f);
	glm::vec3 objectPos = glm::vec3(0.5f, 0.0f, 0.5f);
	objectModel = glm::translate(objectModel,objectPos);
	DazaiEngine::Material floorMat(&shader, tex);
	floorMat.bind();
	floorMat.shader->setMat4("model", objectModel);
	floorMat.shader->setVec4("lightColor", lightColor);
	floorMat.shader->setVec3("lightPos", lightPos);
	

	DazaiEngine::Mesh floor(vert, ind, floorMat);
	DazaiEngine::Mesh light(lightVerts, lightInd,lightMat);


	while (!glfwWindowShouldClose(window)) {
		//timer
		DazaiEngine::Time::updateDeltaTime();
		//camera
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.bind();
		camera.input(window);
		camera.updateMatrix(45.0f,0.1f,100.0f);
		//render
		floor.draw(camera);
		light.draw(camera);
		
		//--
		glfwSwapBuffers(window);
		glfwPollEvents();
		//FPS cap
		DazaiEngine::Time::delayTime();
	
	}
	
	shader.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}
