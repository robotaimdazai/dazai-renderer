#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT

#include "GltfLoader.hpp"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace DazaiEngine
{

	auto  Gltfloader::load(const std::string& filename) -> GltfResult
	{
		tinygltf::TinyGLTF loader;
		tinygltf::Model model;
		std::string err, wrn;
		GltfResult results{};
		auto resolvedFilename = RESOURCES + filename;
		loader.LoadBinaryFromFile(&model, &err, &wrn, resolvedFilename);
		for (auto& mesh : model.meshes)
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
						auto offset = ((char*)NULL + accessor.byteOffset);
			
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
				//INDICES
				//-------
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
				//TANGENTS for normal mapping
				calculateTangents(thisMesh.vertices, thisMesh.indices);
				//TEXTURES
				//---------
				int materialIndex = primitive.material;
				if (materialIndex != -1)
				{
					//baseIndex
					auto material = model.materials[materialIndex];
					thisMesh.name = material.name; // setting mesh name to material name for id for now
					int baseImageIndex = material.pbrMetallicRoughness.baseColorTexture.index;
					int specularImageIndex = material.pbrMetallicRoughness.metallicRoughnessTexture.index;
					int normalImageIndex = material.normalTexture.index;
					//baseColorTex
					if (baseImageIndex != -1)
					{
						auto baseColor = model.images[baseImageIndex];
						Texture2d baseColorTex(&baseColor.image.at(0), baseColor.width, baseColor.height, baseColor.component,
							"diffuse0", 0, Gltfloader::getImageFormat(baseColor), Gltfloader::getImageType(baseColor));
						thisMesh.textures.emplace_back(std::move(baseColorTex));
					}
					//normal maps
					if (normalImageIndex != -1)
					{
						auto normalImage = model.images[normalImageIndex];
						Texture2d normalTexture(&normalImage.image.at(0), normalImage.width, normalImage.height, 3,
							"normal0", 2, GL_RGB, Gltfloader::getImageType(normalImage));
						thisMesh.textures.emplace_back(std::move(normalTexture));
					}
				}
				//push to results
			
				results.meshes.push_back(thisMesh);
			}
		}
		return results;
	}

	auto  Gltfloader::getImageFormat(const tinygltf::Image& image) -> GLenum
	{
		GLenum format = GL_RGBA;
		if (image.component == 1)
		{
			format = GL_RED;
		}
		else if (image.component == 2)
		{
			format = GL_RG;
		}
		else if (image.component == 3)
		{
			format = GL_RGB;
		}

		return format;
	}

	auto  Gltfloader::getImageType(const tinygltf::Image& image) -> GLenum
	{
		GLenum type = GL_UNSIGNED_BYTE;
		if (image.bits == 8)
		{
			// ok
		}
		else if (image.bits == 16)
		{
			type = GL_UNSIGNED_SHORT;
		}
		return type;
	}

	auto Gltfloader::calculateTangent(const Vertex& v0, const Vertex& v1, const Vertex& v2) -> glm::vec3
	{
		glm::vec3 edge1 = v1.position - v0.position;
		glm::vec3 edge2 = v2.position - v0.position;

		glm::vec2 deltaUV1 = v1.uv - v0.uv;
		glm::vec2 deltaUV2 = v2.uv - v0.uv;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		return tangent;
	}

	auto Gltfloader::calculateTangents(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) -> void
	{
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			Vertex& v0 = vertices[indices[i]];
			Vertex& v1 = vertices[indices[i + 1]];
			Vertex& v2 = vertices[indices[i + 2]];

			glm::vec3 tangent = calculateTangent(v0, v1, v2);

			// Accumulate tangent to vertices
			v0.tangent += tangent;
			v1.tangent += tangent;
			v2.tangent += tangent;
		}

		// Orthonormalize and normalize tangents
		for (auto& vertex : vertices)
		{
			vertex.tangent = glm::normalize(vertex.tangent - vertex.normal * glm::dot(vertex.normal, vertex.tangent));
		}
	}

}