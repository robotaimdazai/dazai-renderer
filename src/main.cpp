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

	//draw here

	DazaiEngine::Shader shader("shaders/default.vert", "shaders/default.frag");
	DazaiEngine::Shader lightShader("shaders/light.vert", "shaders/light.frag");

	//pyramid
	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2, // Bottom side
		0, 2, 3, // Bottom side
		4, 6, 5, // Left side
		7, 9, 8, // Non-facing side
		10, 12, 11, // Right side
		13, 15, 14 // Facing side
	};

	GLfloat lightVertices[] =
	{ //     COORDINATES     //
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f
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

	DazaiEngine::Vao vao1;
	vao1.bind();
	DazaiEngine::Vbo vbo1(lightVertices, sizeof(lightVertices));
	DazaiEngine::Ebo ebo1(lightIndices, sizeof(lightIndices));
	vao1.linkAttrib(vbo1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	vao1.unBind();
	vbo1.unBind();
	ebo1.unBind();
	glm::mat4 lightModel = glm::mat4(1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec4 lightColor = { 1.0f,1.0f,1.0f,1.0f };
	lightModel = glm::translate(lightModel, lightPos);
	lightShader.bind();
	lightShader.setMat4("model", lightModel);
	lightShader.setVec4("lightColor", lightColor);

	DazaiEngine::Vao vao;
	vao.bind();
	DazaiEngine::Vbo vbo(vertices, sizeof(vertices));
	DazaiEngine::Ebo ebo(indices,sizeof(indices));
	vao.linkAttrib(vbo,0,3,GL_FLOAT,11*sizeof(float),(void*)0);
	vao.linkAttrib(vbo,1,3,GL_FLOAT,11*sizeof(float),(void*)(3*sizeof(float)));
	vao.linkAttrib(vbo,2,2,GL_FLOAT,11*sizeof(float),(void*)(6*sizeof(float)));
	vao.linkAttrib(vbo,3,3,GL_FLOAT,11*sizeof(float),(void*)(8*sizeof(float)));

	vao.unBind();
	vbo.unBind();
	ebo.unBind();
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	pyramidModel = glm::translate(pyramidModel,pyramidPos);
	shader.bind();
	shader.setMat4("model", pyramidModel);
	shader.setVec4("lightColor", lightColor);
	shader.setVec3("lightPos", lightPos);



	DazaiEngine::Texture2d tex("textures/dazai.jpg",
		GL_TEXTURE_2D,GL_TEXTURE0,
		DazaiEngine::Util::getGlTextureFormatFromExtension(DazaiEngine::FileSystem::getFileExtension("textures/dazai.jpg"))
		,GL_UNSIGNED_BYTE);

	tex.bindToShader(shader,"tex0",0);

	//camera
	DazaiEngine::Camera camera(width, height, glm::vec3(0.0f, 0.0f, 1.0f));

	while (!glfwWindowShouldClose(window)) {
		//timer
		DazaiEngine::Time::updateDeltaTime();
		//render
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.bind();
		camera.input(window);
		camera.updateMatrix(90.0f,0.1f,100.0f);
		camera.setMatrix(shader, "camMatrix");
		vao.bind();
		tex.bind();
		glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		//for light
		lightShader.bind();
		camera.setMatrix(lightShader,"camMatrix");
		vao1.bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		//FPS cap
		DazaiEngine::Time::delayTime();
	}
	vao.destroy();
	vbo.destroy();
	ebo.destroy();
	shader.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}
