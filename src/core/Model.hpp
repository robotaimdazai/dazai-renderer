#pragma once 
#include "Camera.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "../util/GltfLoader.hpp"
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
		auto draw(Camera& camera)->void;
		auto draw(Camera& camera, Material& material)->void;
		auto draw(Camera& camera, Shader& shader)->void;
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		Transform transform;
		std::vector<glm::mat4> trsMatrices;
	};
}