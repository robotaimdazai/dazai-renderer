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
#include "core/FrameBuffer.hpp"
#include "core/FrameBufferTexture2d.hpp"
#include "core/RenderBuffer.hpp"
#include "core/TextureCubemap.hpp"
#include "core/Skybox.hpp"
#include "imgui/imgui.h";
#include "imgui/backends/imgui_impl_opengl3.h";
#include "imgui/backends/imgui_impl_glfw.h";

const unsigned int width = 800;
const unsigned int height = 800;

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
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init("#version 130");

	gladLoadGL();

	
	//glfwSwapInterval(0);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glViewport(0,0,width,height); 
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
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
	DazaiEngine::Shader instancedShader("shaders/defaultinstanced.vert", "shaders/default.frag", "shaders/default.geom");
	DazaiEngine::Shader lightShader("shaders/light.vert", "shaders/light.frag");
	DazaiEngine::Shader outlineShader("shaders/outline.vert", "shaders/light.frag");
	DazaiEngine::Shader frameBufferShader("shaders/framebuffer.vert", "shaders/framebuffer.frag");
	DazaiEngine::Shader blurShader("shaders/framebuffer.vert", "shaders/blur.frag");
	DazaiEngine::Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

	frameBufferShader.bind();
	frameBufferShader.setInt("screenTexture", 0);
	frameBufferShader.setInt("bloomTexture", 1);
	blurShader.bind();
	blurShader.setInt("screenTexture", 0);

	//camera
	DazaiEngine::Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	DazaiEngine::TextureCubemap skyboxTex
	(
		"textures/right.jpg",
		"textures/left.jpg",
		"textures/top.jpg",
		"textures/bottom.jpg",
		"textures/front.jpg",
		"textures/back.jpg"
	);
	skyboxShader.bind();
	skyboxShader.setInt("skybox", 0);
	//data
	//light
	std::vector<DazaiEngine::Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(DazaiEngine::Vertex));
	std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	
	//create scene
	DazaiEngine::Scene scene;
	DazaiEngine::Transform lightTransform;
	lightTransform.position = { 0.5,0.5f,1.5f };
	scene.lightColor = { 1.0f,1.0f,1.0f,1.0f };
	scene.lightPos = lightTransform.position;

	//meshes
	DazaiEngine::Mesh light(lightVerts, lightInd);
	//models
	//for instancing
	// The number of asteroids to be created
	const unsigned int number = 100;
	// Radius of circle around which asteroids orbit
	float radius = 10;
	// How much ateroids deviate from the radius
	float radiusDeviation = 0;
	// Holds all transformations for the asteroids
	std::vector <glm::mat4> instanceMatrix;
	for (unsigned int i = 0; i < number; i++)
	{
		// Generates x and y for the function x^2 + y^2 = radius^2 which is a circle
		float randf = -1.0f + (rand() / (RAND_MAX / 2.0f));
		float x = randf;
		float finalRadius = radius + randf * radiusDeviation;
		float y = ((rand() % 2) * 2 - 1) * sqrt(1.0f - x * x);

		// Holds transformations before multiplying them
		glm::vec3 tempTranslation = {0,0,0};
		glm::quat tempRotation = {0,0,0,0};
		glm::vec3 tempScale = glm::vec3(1.0,1.0,1.0);

		// Makes the random distribution more even
		if (randf > 0.5f)
		{
			// Generates a translation near a circle of radius "radius"
			tempTranslation = glm::vec3(y * finalRadius, randf, x * finalRadius);
		}
		else
		{
			// Generates a translation near a circle of radius "radius"
			tempTranslation = glm::vec3(x * finalRadius, randf, y * finalRadius);
		}
		// Generates random rotations
		//tempRotation = glm::quat(1.0f, randf, randf, randf);
		// Generates random scales


		// Initialize matrices
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);

		// Push matrix transformation
		instanceMatrix.push_back(trans * rot * sca);
	}
	DazaiEngine::Model model("models/cs.glb", &shader);
	model.transform.position = { 0,0,0 };
	model.transform.rotation = { 0,0,0,0 };
	//skybox
	DazaiEngine::Skybox skybox;
	//model.transform.scale = { 1,1,1 };
	// -------------------------------------------------------------------------------
	
	//framebuffer
	DazaiEngine::FrameBuffer mainFrameBuffer;
	mainFrameBuffer.createVaoVbo();
	DazaiEngine::FrameBufferTexture2d mainTex(width,height,GL_COLOR_ATTACHMENT0,GL_RGB16F,GL_RGB, GL_UNSIGNED_BYTE);
	DazaiEngine::FrameBufferTexture2d bloomTexture(width,height,GL_COLOR_ATTACHMENT1,GL_RGB16F,GL_RGB, GL_UNSIGNED_BYTE);
	GLenum attachments[2] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1 };
	mainFrameBuffer.setDrawBuffer(2,attachments);
	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;

	DazaiEngine::RenderBuffer rb(width,height);
	mainFrameBuffer.unbind();
	//pingpong buffer for bloom
	DazaiEngine::FrameBuffer ping;
	DazaiEngine::FrameBufferTexture2d pingTex(width,height, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGB, GL_UNSIGNED_BYTE);
	DazaiEngine::FrameBuffer pong;
	DazaiEngine::FrameBufferTexture2d pongTex(width, height, GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGB, GL_UNSIGNED_BYTE);
	unsigned int pingpongFBO[2] = {ping.id,pong.id};
	DazaiEngine::FrameBufferTexture2d* pingpongBuffer[2] = { &pingTex,&pongTex};
	

	while (!glfwWindowShouldClose(window)) {
		//timer
		DazaiEngine::Time::updateDeltaTime();
		std::string windowText = "FPS: " + std::to_string(DazaiEngine::Time::fps())+ 
			" MS: " + std::to_string(DazaiEngine::Time::deltaTime * 1000.0f);
		glfwSetWindowTitle(window, windowText.c_str());
		
		mainFrameBuffer.bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		glEnable(GL_DEPTH_TEST);
		//camera
		shader.bind();
		camera.input(window);
		camera.updateMatrix(45.0f,0.1f,100.0f);
		//render
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		model.draw(camera,scene);
		glStencilFunc(GL_NOTEQUAL,1, 0xff);
		glDisable(GL_DEPTH_TEST);
		//outlineShader.bind();
		//outlineShader.setFloat("outlining", 0.02f);
		//model.draw(camera, scene, outlineMaterial);
		glStencilFunc(GL_ALWAYS, 0, 0xff);
		glEnable(GL_DEPTH_TEST);
		//draw skybox
		skybox.draw(skyboxShader, skyboxTex, camera);
		//light.draw(lightShader, tex,camera,scene, lightTransform.position,lightTransform.rotation,lightTransform.scale);
		//BLOOM-----------
		bool horizontal = true, first_iteration = true;
		int amount = 2;
		blurShader.bind();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			blurShader.setInt("horizontal", horizontal);
			// In the first bounc we want to get the data from the bloomTexture
			if (first_iteration)
			{
				glBindTexture(GL_TEXTURE_2D, bloomTexture.id);
				first_iteration = false;
			}
			// Move the data between the pingPong textures
			else
			{
				glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]->id);
			}
			mainFrameBuffer.bindVao();
			glDisable(GL_DEPTH_TEST); // dont do on framebuffer Rect
			glDrawArrays(GL_TRIANGLES, 0, 6);
			horizontal = !horizontal;	
		}
		//---------------
		mainFrameBuffer.unbind();
		frameBufferShader.bind();
		mainFrameBuffer.bindVao();
		glDisable(GL_DEPTH_TEST); // dont do on framebuffer Rect
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mainTex.id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]->id);
		glDrawArrays(GL_TRIANGLES,0,6);
		
	
		// 
		//IMGUI---------------------------------------------------------
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//----IMGUI Rendering-----------
		//
				ImGui::ShowDemoWindow();

		//
		//------------------------------
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//-----------------------------------------------------------
		// 
		glfwSwapBuffers(window);
		glfwPollEvents();
		//FPS cap
		DazaiEngine::Time::delayTime();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
