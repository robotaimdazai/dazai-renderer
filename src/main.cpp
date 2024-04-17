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
#include "core/FrameBuffer.hpp"
#include "core/FrameBufferTexture2d.hpp"
#include "core/RenderBuffer.hpp"
#include "core/TextureCubemap.hpp"
#include "core/Skybox.hpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "core/Light.hpp"

const unsigned int width = 800;
const unsigned int height = 800;

int main()
{
#pragma region SCENE SETUP
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
	DazaiEngine::Resources::loadAllShaders();
	//shaders
	auto shader = DazaiEngine::Resources::getShader(SHADER_DEFAULT);
	auto shaderInstanced= DazaiEngine::Resources::getShader(SHADER_DEFAULT_INSTANCED);
	auto lightShader = DazaiEngine::Resources::getShader(SHADER_LIGHT);
	auto outlineShader =DazaiEngine::Resources::getShader(SHADER_OUTLINE);
	auto frameBufferShader = DazaiEngine::Resources::getShader(SHADER_FRAMEBUFFER);
	auto blurShader = DazaiEngine::Resources::getShader(SHADER_BLUR);
	auto skyboxShader = DazaiEngine::Resources::getShader(SHADER_SKYBOX);
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
#pragma endregion

#pragma region INSTANCING EXAMPLE

	//models
	//for instancing
	// The number of asteroids to be created
	const unsigned int number = 500;
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
	//DazaiEngine::Model model("models/cs.glb", &shaderInstanced,number,instanceMatrix);
#pragma endregion

#pragma region FRAMEBUFFER SETUP

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
#pragma endregion

	DazaiEngine::Skybox skybox;
	DazaiEngine::Model model("models/cs.glb", &shader);
	DazaiEngine::Light light;
	//DEFAULT PROPERTIES
	light.transform.position =  { 0,2.7f,2.3f };
	//default
	bool normalMap =			0;
	bool ao =					0;
	bool blinPhong =			1;
	float ambient =				0.3f;
	float specular =			0.8f;
	float specularPower =		128;
	int lightType =				0;
	//framebuffer
	float gamma =				2.2f;
	float exposure =			2;
	float bloomIntensity =		0.5f;
	bool bloom =				0;
	bool hdr =					0;

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
		camera.input(window);
		camera.updateMatrix(45.0f,0.1f,100.0f);
		//render
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		//framebuffer props
		frameBufferShader.bind();
		frameBufferShader.setFloat("gamma",gamma);
		frameBufferShader.setFloat("exposure",exposure);
		frameBufferShader.setFloat("bloomIntensity",bloomIntensity);
		frameBufferShader.setBool("useHdr",hdr);
		frameBufferShader.setBool("useBloom",bloom);
		//default sahder props
		shader.bind();
		shader.setBool("useNormalMap", normalMap);
		shader.setBool("useAmbientOcclusion", ao);
		shader.setBool("blinnPhong", blinPhong);
		shader.setFloat("ambient", ambient);
		shader.setFloat("specularLight", specular);
		shader.setFloat("specPower", specularPower);
		shader.setVec3("lightPos", light.transform.position );
		shader.setVec4("lightColor", light.color );
		shader.setInt("lightType", lightType );
		model.draw(camera,shader);
		glStencilFunc(GL_NOTEQUAL,1, 0xff);
		glDisable(GL_DEPTH_TEST);
		//outlineShader.bind();
		//outlineShader.setFloat("outlining", 0.02f);
		//model.draw(camera,outlineShader);
		glStencilFunc(GL_ALWAYS, 0, 0xff);
		glEnable(GL_DEPTH_TEST);
		light.draw(camera,lightShader);
		//draw skybox
		skybox.draw(skyboxShader, skyboxTex, camera);
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
		ImGui::Begin("Scene");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Light");
		ImGui::Text("0:Directional");
		ImGui::Text("1:Point");
		ImGui::Text("2:Spot");
		ImGui::SliderInt("Light Type",&lightType,0,2);
		ImGui::Checkbox("Blin Phong / Phong", &blinPhong);
		ImGui::SliderFloat3("Light Pos", &light.transform.position.x,-10,10);
		ImGui::SliderFloat3("Light Color", &light.color.x,0,1);
		ImGui::SliderFloat("Ambient", &ambient,0,1);
		ImGui::SliderFloat("Specular", &specular,0,1);
		ImGui::SliderFloat("Specular Power", &specularPower,0,512);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Render");
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Checkbox("Normal Map", &normalMap);
		ImGui::Checkbox("Ambient Occlusion", &ao);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Post Processing");
		ImGui::Checkbox("HDR", &hdr);
		ImGui::Checkbox("Bloom", &bloom);
		ImGui::SliderFloat("Bloom Intensity", &bloomIntensity,0,10);
		ImGui::SliderFloat("Gamma", &gamma,0,5);
		ImGui::SliderFloat("Exposure", &exposure,0,100);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::End();

		//
		//------------------------------
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//-----------------------------------------------------------
		// 
		glfwSwapBuffers(window);
		glfwPollEvents();
		//FPS cap
		//DazaiEngine::Time::delayTime();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
