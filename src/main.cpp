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
	DazaiEngine::Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
	DazaiEngine::Shader shadowmapShader("shaders/shadowmap.vert", "shaders/shadowmap.frag");
	//camera
	DazaiEngine::Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
	//textures
	DazaiEngine::Texture2d textures[]
	{
		DazaiEngine::Texture2d("textures/planks.png","diffuse0", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		DazaiEngine::Texture2d("textures/planksSpec.png","specular0", 1, GL_RED, GL_UNSIGNED_BYTE)
	};
	DazaiEngine::TextureCubemap skyboxTex
	(
		"textures/right.jpg",
		"textures/left.jpg",
		"textures/top.jpg",
		"textures/bottom.jpg",
		"textures/front.jpg",
		"textures/back.jpg"
	);
	skyboxTex.bindToSlot(skyboxShader,"skybox");
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
	lightTransform.position = { 0.5,0.5f,0.5f };
	scene.lightColor = { 1.0f,1.0f,1.0f,1.0f };
	scene.lightPos = lightTransform.position;

	DazaiEngine::Material lightMat(&lightShader, tex);
	DazaiEngine::Material outlineMaterial(&outlineShader, tex);
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
	DazaiEngine::FrameBuffer fb;
	fb.createVaoVbo();
	DazaiEngine::FrameBufferTexture2d fbTex(width,height,GL_COLOR_ATTACHMENT0,GL_RGB16F,GL_RGB, GL_UNSIGNED_BYTE,0);
	fbTex.bindToSlot(frameBufferShader,"screenTexture");
	DazaiEngine::RenderBuffer rb(width,height);
	fb.unbind();

	// shadowMap Framebuffer
	DazaiEngine::FrameBuffer smfb;
	DazaiEngine::FrameBufferTexture2d smTex(2048, 2048, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT,
		GL_DEPTH_COMPONENT, GL_FLOAT, 2);
	smfb.setDrawBuffer(GL_NONE);
	smfb.setReadBuffer(GL_NONE);
	smfb.unbind();
	//light perspective for shadows
	glm::mat4 lightProj = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 100.0f);
	glm::mat4 lightView = glm::lookAt(  20.0f * scene.lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	scene.lightProjection = lightProj * lightView;
	shadowmapShader.bind();
	shadowmapShader.setMat4(scene.LIGHT_PROJECTION_UNIFORM, scene.lightProjection);
	//core loop
	while (!glfwWindowShouldClose(window)) {
		//timer
		DazaiEngine::Time::updateDeltaTime();
		std::string windowText = "FPS: " + std::to_string(DazaiEngine::Time::fps())+ 
			" MS: " + std::to_string(DazaiEngine::Time::deltaTime * 1000.0f);
		glfwSetWindowTitle(window, windowText.c_str());
		//shadowmaps
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, 2048, 2048);
		smfb.bind();
		glClear(GL_DEPTH_BUFFER_BIT);
		model.draw(camera, shadowmapShader,scene);
		smfb.unbind(); // at this stage depth buffer will contain depth data for drawn model
		glViewport(0, 0, width, height);
		//framebuffer
		fb.bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		glEnable(GL_DEPTH_TEST);
		//camera
		shader.bind();
		camera.input(window);
		camera.updateMatrix(45.0f,0.1f,100.0f);
		//render
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		//set shadowmapTexure
		smTex.bind();
		smTex.bindToSlot(shader, scene.SHADOW_MAP_UNIFORM);
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
		fb.unbind();
		frameBufferShader.bind();
		fb.bindVao();
		glDisable(GL_DEPTH_TEST); // dont do on framebuffer Rect
		fbTex.bind();
		glDrawArrays(GL_TRIANGLES,0,6);
		
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
