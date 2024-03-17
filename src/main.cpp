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


	// Vertices coordinates
	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	DazaiEngine::Vao vao;
	vao.bind();

	DazaiEngine::Vbo vbo(vertices, sizeof(vertices));
	DazaiEngine::Ebo ebo(indices,sizeof(indices));

	vao.linkAttrib(vbo,0,3,GL_FLOAT,8*sizeof(float),(void*)0);
	vao.linkAttrib(vbo,1,3,GL_FLOAT,8*sizeof(float),(void*)(3*sizeof(float)));
	vao.linkAttrib(vbo,2,2,GL_FLOAT,8*sizeof(float),(void*)(6*sizeof(float)));

	vao.unBind();
	vbo.unBind();
	ebo.unBind();

	DazaiEngine::Texture2d tex("textures/dazai.jpg",
		GL_TEXTURE_2D,GL_TEXTURE0,
		DazaiEngine::Util::getGlTextureFormatFromExtension(DazaiEngine::FileSystem::getFileExtension("textures/dazai.jpg"))
		,GL_UNSIGNED_BYTE);

	tex.bindToShader(shader,"tex0",0);
	float rot = 0.0f;
	double prevTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.bind();

		double curTime = glfwGetTime();
		if (curTime - prevTime >= 1/60)
		{
			rot += 0.2f;
			prevTime = curTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		

		model = glm::rotate(model,glm::radians(rot),glm::vec3(0,1.0f,0));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
		proj = glm::perspective(glm::radians(70.0f), (float)(width/height), 0.1f, 100.0f);
		shader.setMat4("model",model);
		shader.setMat4("view",model);
		shader.setMat4("proj",model);
		vao.bind();
		tex.bind();
		//glDrawArrays(GL_TRIANGLES,0,3);
		glDrawElements(GL_TRIANGLES,sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	vao.destroy();
	vbo.destroy();
	ebo.destroy();
	shader.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}
