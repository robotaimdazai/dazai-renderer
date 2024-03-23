#pragma once
#include "Shader.hpp"
#include "Texture2d.hpp"
#include "Camera.hpp"
#include <vector>

namespace DazaiEngine 
{
	class Material
	{
	public:
		Material() = default;
		Material(Shader* shader);
		Material(Shader* shader,std::vector<Texture2d>& textures);
		auto bind() -> void;
		auto unbind() -> void;
		Shader* shader;
		std::vector<Texture2d> textures;
	};
}