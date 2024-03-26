#include "Texture2d.hpp";

namespace DazaiEngine
{
	Texture2d::Texture2d(const std::string& path, const char* texType, unsigned int slot, GLenum format, GLenum pixelType):
		texType(texType),slot(slot),id(-1)
	{
		//generations
		unsigned char* bytes = Resources::loadRawTexture(path, &width, &height, &numChannels, 0);
		glGenTextures(1,&id);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D,id);
		//parameters
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,format,pixelType,bytes);
		//mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(bytes);
		glBindTexture(GL_TEXTURE_2D,0);

	}
	Texture2d::Texture2d( unsigned char* bytes, int width, int height, int numChannels ,const char* texType, unsigned int slot, GLenum format, GLenum pixelType):
		texType(texType), slot(slot), id(-1), numChannels(numChannels),width(width),height(height)
	{
		//generations
		glGenTextures(1, &id);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
		//parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, pixelType, bytes);
		//mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	auto Texture2d::bindToSlot(Shader& shader, const char* uniform, unsigned int slot) -> void
	{
		shader.bind();
		shader.setInt(uniform, GL_TEXTURE0 + slot);
	}
	auto Texture2d::bind() -> void
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	auto Texture2d::unbind() -> void
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	auto Texture2d::destroy() -> void
	{
		glDeleteTextures(1,&id);
	}

}