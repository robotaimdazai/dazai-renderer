#include "TextureCubemap.hpp"
#include "TextureCubemap.hpp"

namespace DazaiEngine
{
	TextureCubemap::TextureCubemap
	(
		const std::string& right,
		const std::string& left, 
		const std::string& top, 
		const std::string& bottom, 
		const std::string& front, 
		const std::string& back 
	)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// These are very important to prevent seams
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		std::string facesCubemap[6] =
		{
			right, left, top, bottom, front, back
		};
		for (unsigned int i = 0; i < 6; i++)
		{
			
			unsigned char* data =
				Resources::loadRawTexture(facesCubemap[i].c_str(), &width, &height,
					&numChannels,0, false);

			if (data)
			{
				
				glTexImage2D
				(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_SRGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
				stbi_image_free(data);
			}
		}
	}
	auto TextureCubemap::bind() -> void
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	}
	auto TextureCubemap::bindToSlot(Shader& shader, const char* uniform) -> void
	{
		shader.bind();
		shader.setInt(uniform, GL_TEXTURE0);
	}
	auto TextureCubemap::unbind() -> void
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}