#pragma once
#define TINYGLTF_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#include "tiny_gltf.h"
#include "../core/Vbo.hpp"
#include "../core/Texture2d.hpp"

namespace DazaiEngine
{
	struct GltfMesh
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture2d> textures;
	};
	struct GltfResult
	{
		std::vector<GltfMesh> meshes;
	};
	class Gltfloader
	{
	public:
		auto static load(const std::string& filename) -> GltfResult
		{
			tinygltf::TinyGLTF loader;
			tinygltf::Model model;
			std::string err, wrn;
			GltfResult results{};
			auto resolvedFilename = RESOURCES + filename;
			loader.LoadBinaryFromFile(&model, &err, &wrn, resolvedFilename);
			for (auto& mesh: model.meshes)
			{
				for (size_t i = 0; i < mesh.primitives.size(); i++)
				{
					auto primitive = mesh.primitives[i];
					std::vector<float> vertices;
					std::vector<float> uvs;
					std::vector<float> normals;
					std::vector<unsigned int> indices;
					GltfMesh thisMesh{};
					if (primitive.mode != TINYGLTF_MODE_TRIANGLES) throw;
					for (auto& attrib : primitive.attributes)
					{
						if (attrib.first == "POSITION")
						{
							auto& accessor = model.accessors[attrib.second];
							auto& bufferView = model.bufferViews[accessor.bufferView];
							auto type = accessor.type;
							auto byteStride = accessor.ByteStride(bufferView);
							auto size = accessor.count;
							auto& buffer = model.buffers[bufferView.buffer];
							for (size_t i = 0; i < size; i++)
							{//x,y,z
								vertices.push_back(
									*(reinterpret_cast<float*>(&(buffer.data[(i * byteStride + bufferView.byteOffset)])))
								);
								vertices.push_back(
									*(reinterpret_cast<float*>(&(buffer.data[(i * byteStride + bufferView.byteOffset) + sizeof(float)])))
								);
								vertices.push_back(
									*(reinterpret_cast<float*>(&(buffer.data[(i * byteStride + bufferView.byteOffset) + sizeof(float) * 2])))
								);
							}
						}
						else if (attrib.first == "TEXCOORD_0")
						{
							auto& accessor = model.accessors[attrib.second];
							auto& bufferView = model.bufferViews[accessor.bufferView];
							auto type = accessor.type;
							auto byteStride = accessor.ByteStride(bufferView);
							auto size = accessor.count;
							auto& buffer = model.buffers[bufferView.buffer];

							for (auto i = 0; i < size; ++i)
							{
								uvs.push_back(
									*(reinterpret_cast<float*>(&(buffer.data[(i * byteStride + bufferView.byteOffset)])))
								);
								uvs.push_back(
									*(reinterpret_cast<float*>(&(buffer.data[(i * byteStride + bufferView.byteOffset) + sizeof(float)])))
								);
							}
						}
						else if (attrib.first == "NORMAL")
						{
							auto& accessor = model.accessors[attrib.second];
							auto& bufferView = model.bufferViews[accessor.bufferView];
							auto type = accessor.type;
							auto byteStride = accessor.ByteStride(bufferView);
							auto size = accessor.count;
							auto& buffer = model.buffers[bufferView.buffer];
							for (auto i = 0; i < size; ++i)
							{
								normals.push_back(
									*(reinterpret_cast<float*>(&(buffer.data[(i * byteStride + bufferView.byteOffset)])))
								);
								normals.push_back(
									*(reinterpret_cast<float*>(&(buffer.data[(i * byteStride + bufferView.byteOffset) + sizeof(float)])))
								);
								normals.push_back(
									*(reinterpret_cast<float*>(&(buffer.data[(i * byteStride + bufferView.byteOffset) + sizeof(float) * 2])))
								);
							}
						}
					}
					// indices
					auto& indexAccessor = model.accessors[primitive.indices];
					auto& indexBufferView = model.bufferViews[indexAccessor.bufferView];
					auto indexStride = indexAccessor.ByteStride(indexBufferView);
					auto& indexBuffer = model.buffers[indexBufferView.buffer];
					for (auto i = 0; i < indexAccessor.count; ++i)
					{
						auto val = *(reinterpret_cast<unsigned int*>(&(indexBuffer.data[i * indexStride + indexBufferView.byteOffset]))) & ((~0u) >> 16);
						thisMesh.indices.push_back(val);
					}
					auto it_vert = vertices.begin();
					auto it_uv = uvs.begin();
					auto it_normal = normals.begin();
					while (it_vert != vertices.end() && it_uv != uvs.end() && it_normal != normals.end())
					{
						Vertex vertex;
						float x = *(it_vert++);
						float y = *(it_vert++);
						float z = *(it_vert++);
						vertex.position = { x,y,z };
						float u = *(it_uv++);
						float v = *(it_uv++);
						vertex.uv = { u,v };
						float nx = *(it_normal++);
						float ny = *(it_normal++);
						float nz = *(it_normal++);
						vertex.normal = { nx,ny,nz };
						//vertex color are unlikely so puting white for now
						vertex.color = { 1.0f,1.0f,1.0f };
						thisMesh.vertices.push_back(vertex);
					}
					results.meshes.push_back(thisMesh);
				}
				
			}
			return results;
		}
		
	};
} 