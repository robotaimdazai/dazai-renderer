#pragma once 
#include "Camera.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "../util/GltfLoader.hpp"
#include "Scene.hpp"
#include "Transform.hpp"

namespace DazaiEngine
{
	class Model
	{
	public:
		Model() = delete;
		Model(const char* file, Shader* shader, unsigned int instances = 1, std::vector<glm::mat4> trsMatrices = {});
		Model(std::vector<Mesh>& meshes, std::vector<Material>& materials);
		Model(Mesh& mesh,Material& material);
		auto draw(Camera& camera, const Scene& scene)->void;
		auto draw(Camera& camera, const Scene& scene, Material& material)->void;
		auto draw(Camera& camera, Shader& shader, Scene& scene)->void;
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		Transform transform;
		std::vector<glm::mat4> trsMatrices;
	};
}