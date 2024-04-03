#pragma once
#include "Vao.hpp"
#include "Vbo.hpp"
#include "Ebo.hpp"
#include "TextureCubemap.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include <glad/glad.h>

namespace DazaiEngine
{
	class Skybox
	{
	public:
		Skybox();
		auto draw(Shader& shader,TextureCubemap& texture, Camera& camera) -> void;
		unsigned int vao;
		unsigned int vbo;
		unsigned int ebo;
	};
}