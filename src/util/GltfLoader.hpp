#pragma once
#include "../core/Vbo.hpp"
#include "../core/Texture2d.hpp"
#include <tiny_gltf.h>
namespace DazaiEngine
{
	struct GltfMesh
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture2d> textures;
		std::string name;
	};
	struct GltfResult
	{
		std::vector<GltfMesh> meshes;
	};
	class Gltfloader
	{
	public:
		auto static load(const std::string& filename)->GltfResult;
	private:
		auto static getImageFormat(const tinygltf::Image& image)->GLenum;
		auto static getImageType(const tinygltf::Image& image)->GLenum;
		auto static calculateTangent(const Vertex& v0, const Vertex& v1, const Vertex& v2)->glm::vec3;
		auto static calculateTangents(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) ->void;

	};
}