// DazaiRenderer.cpp : Defines the entry point for the application.
//
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Shader.hpp"
#include "core/Vao.hpp"
#include "core/Vbo.hpp"
#include "core/Ebo.hpp"
#include "stb/stb_image.h";
#include "core/Texture2d.hpp"


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

	glViewport(0,0,800,600); 

	//draw here

	DazaiEngine::Shader shader("shaders/default.vert", "shaders/default.frag");


	GLfloat vertices[] =
	{ //     COORDINATES     /        COLORS      /   TexCoord  //
		-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
	};

	GLuint indices[] =
	{
		0, 2, 1, // Upper triangle
		0, 3, 2 // Lower triangle
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

	DazaiEngine::Texture2d tex("textures/dazai.png",GL_TEXTURE_2D,GL_TEXTURE0,GL_RGBA,GL_UNSIGNED_BYTE);
	tex.bindToShader(shader,"tex0",0);


	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.bind();
		vao.bind();
		tex.bind();
		//glDrawArrays(GL_TRIANGLES,0,3);
		glDrawElements(GL_TRIANGLES,9,GL_UNSIGNED_INT,0);

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
