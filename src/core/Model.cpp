#include "Model.hpp"

namespace DazaiEngine
{
	
	Model::Model(const char* file)
	{

	}
	Model::Model(Mesh& mesh):mesh(mesh)
	{
	}
	Model::Model(const char* file, Material& material):
		material(material)
	{

	}
	Model::Model(Material& material):
		material(material)
	{

	}
	Model::Model(Mesh& mesh, Material& material) :
		mesh(mesh), material(material)
	{

	}
	auto Model::draw(Camera& camera) -> void
	{
		mesh.draw(*material.shader, material.textures, camera);
	}

}