#include "Material.hpp"

namespace DazaiEngine
{
	Material::Material(Shader* shader):
		shader(shader)
	{
	}
	Material::Material(Shader* shader, std::vector<Texture2d>& textures):
		shader(shader),textures(textures)
	{
	}
	auto Material::bind() -> void
	{
		if(shader!=nullptr)
			shader->bind();
		for (size_t i = 0; i < textures.size(); i++)
		{
			textures[i].bind();
		}
	}
	auto Material::unbind() -> void
	{
		if (shader != nullptr)
			shader->unbind();
		for (size_t i = 0; i < textures.size(); i++)
		{
			textures[i].unbind();
		}
	}
}