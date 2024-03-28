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
		Model() = default;
		Model(const char* file, Shader* shader);
		Model(std::vector<Mesh>& meshes, std::vector<Material>& materials);
		Model(Mesh& mesh,Material& material);
		auto draw(Camera& camera, const Scene& scene)->void;
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		Transform transform;
	};
}