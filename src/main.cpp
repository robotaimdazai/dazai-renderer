// DazaiRenderer.cpp : Defines the entry point for the application.
//
#include <iostream>
#include "glad/glad.h"
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
#include "core/Camera.hpp"
#include "core/Time.hpp"
#include "core/Mesh.hpp"
#include "core/Material.hpp"
#include "core/Model.hpp"
#include "core/Scene.hpp"


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
	{
		//               COORDINATES          /       TEXTURE COORDINATES    /           NORMALS         /            COLORS          //
		DazaiEngine::Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
		DazaiEngine::Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
		DazaiEngine::Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
		DazaiEngine::Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)}
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
	//load glb
	//auto results = DazaiEngine::Gltfloader::load("models/csgo.glb");

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
	//data
	//floor
	std::vector<DazaiEngine::Vertex> vert(vertices, vertices + sizeof(vertices) / sizeof(DazaiEngine::Vertex));
	std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<DazaiEngine::Texture2d> tex(textures, textures + sizeof(textures) / sizeof(DazaiEngine::Texture2d));
	//light
	std::vector<DazaiEngine::Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(DazaiEngine::Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	
	//create scene
	DazaiEngine::Scene scene;
	DazaiEngine::Transform lightTransform;
	lightTransform.position = { 0.5,2.5f,0.5 };
	scene.lightColor = { 1.0f,1.0f,1.0f,1.0f };
	scene.lightPos = lightTransform.position;

	DazaiEngine::Material lightMat(&lightShader, tex);
	//meshes
	DazaiEngine::Mesh light(lightVerts, lightInd);
	//models
	DazaiEngine::Model model("models/cs.glb", &shader);
	model.transform.position = { 0,0,0 };
	model.transform.rotation = { 0,0,0,0 };
	//model.transform.scale = { 1,1,1 };
	//core loop
	while (!glfwWindowShouldClose(window)) {
		//timer
		DazaiEngine::Time::updateDeltaTime();
		//camera
		glClearColor(0.85f, 0.85f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.bind();
		camera.input(window);
		camera.updateMatrix(45.0f,0.1f,100.0f);
		//render
		model.draw(camera,scene);
		light.draw(lightShader, tex,camera,scene, 
			lightTransform.position,lightTransform.rotation,lightTransform.scale);
		//--
		glfwSwapBuffers(window);
		glfwPollEvents();
		//FPS cap
		DazaiEngine::Time::delayTime();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
