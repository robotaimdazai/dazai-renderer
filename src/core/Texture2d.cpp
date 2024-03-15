#include "Texture2d.hpp";

namespace DazaiEngine
{
	Texture2d::Texture2d(const std::string& path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
	{
		//generations
		mType = texType;
		mSlot = slot;
		stbi_set_flip_vertically_on_load(true);
		std::string resolvedPath = RESOURCES + path;
		unsigned char* bytes = stbi_load(resolvedPath.c_str(), &mWidth, &mHeight, &mNumChannels, 0);
		glGenTextures(1,&mId);
		glActiveTexture(slot);
		glBindTexture(texType,mId);
		//parameters
		glTexParameteri(texType,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(texType,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(texType,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(texType,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexImage2D(texType,0,GL_RGBA,mWidth,mHeight,0,format,pixelType,bytes);
		//mipmaps
		glGenerateMipmap(texType);
		//
		stbi_image_free(bytes);
		glBindTexture(texType,0);

	}
	auto Texture2d::bindToShader(Shader& shader, const char* uniform, GLuint slot) -> void
	{
		shader.bind();
		shader.setInt(uniform, slot);
	}
	auto Texture2d::bind() -> void
	{
		glBindTexture(mType, mId);
	}
	auto Texture2d::unbind() -> void
	{
		glBindTexture(mType, 0);
	}
	auto Texture2d::destroy() -> void
	{
		glDeleteTextures(1,&mId);
	}
	auto Texture2d::id() -> unsigned int
	{
		return mId;
	}
	auto Texture2d::width() ->  int
	{
		return mWidth;
	}
	auto Texture2d::height() ->  int
	{
		return mHeight;
	}
	auto Texture2d::numChannels() ->  int
	{
		return mNumChannels;
	}
	auto Texture2d::type() ->  GLenum
	{
		return mType;
	}
	auto Texture2d::slot() -> unsigned int
	{
		return mSlot;
	}
}