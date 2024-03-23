#pragma once 
#include "Camera.hpp"
#include "Material.hpp"
#include "Mesh.hpp"


namespace DazaiEngine
{
	class Model
	{
	public:
		Model() = default;
		Model(const char* file);
		Model(Mesh& mesh);
		Model(Mesh& mesh, Material& material);
		Model(const char* file, Material& material);
		Model(Material& material);
		auto draw(Camera& camera)->void;
		Mesh mesh;
		Material material;
	};
}