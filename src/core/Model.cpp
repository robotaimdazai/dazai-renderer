#include "Model.hpp"

namespace DazaiEngine
{
	
	Model::Model(const char* file, Shader* shader)
	{
		auto result = Gltfloader::load(file);
		for (size_t i = 0; i < result.meshes.size(); i++)
		{
			Mesh thisMesh(result.meshes[i].vertices, result.meshes[i].indices);
			Material thisMaterial(shader,result.meshes[i].textures);
			meshes.emplace_back(std::move(thisMesh));
			materials.emplace_back(std::move(thisMaterial));
		}
	}

	Model::Model(std::vector<Mesh>& meshes, std::vector<Material>& materials):
		materials(materials),meshes(meshes)
	{
	}
	Model::Model(Mesh& mesh, Material& material)
	{
		meshes.push_back(mesh);
		materials.push_back(material);
	}
	auto Model::draw(Camera& camera, const Scene& scene) -> void
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw(*materials[i].shader, materials[i].textures, camera, scene,
				transform.position,transform.rotation,transform.scale);
		}
	}

	auto Model::draw(Camera& camera, const Scene& scene, Material& material) -> void
	{
		for (size_t i = 0; i < meshes.size(); i++)
		{
			meshes[i].draw(*material.shader, material.textures, camera, scene,
				transform.position, transform.rotation, transform.scale);
		}
	}

}