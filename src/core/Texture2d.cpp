#include "Texture2d.hpp";

namespace DazaiEngine
{
	Texture2d::Texture2d(const std::string& path, GLenum texType, unsigned int slot, GLenum format, GLenum pixelType):
		texType(texType),slot(slot),id(-1)
	{
		//generations
		unsigned char* bytes = Resources::loadRawTexture(path, &width, &height, &mNumChannels, 0);
		glGenTextures(1,&id);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(texType,id);
		//parameters
		glTexParameteri(texType,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(texType,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(texType,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(texType,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexImage2D(texType,0,GL_RGBA,width,height,0,format,pixelType,bytes);
		//mipmaps
		glGenerateMipmap(texType);
		stbi_image_free(bytes);
		glBindTexture(texType,0);

	}
	auto Texture2d::bindToShader(Shader& shader, const char* uniform, unsigned int slot) -> void
	{
		shader.bind();
		shader.setInt(uniform, GL_TEXTURE0 + slot);
	}
	auto Texture2d::bind() -> void
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(texType, id);
	}
	auto Texture2d::unBind() -> void
	{
		glBindTexture(texType, 0);
	}
	auto Texture2d::destroy() -> void
	{
		glDeleteTextures(1,&id);
	}

}