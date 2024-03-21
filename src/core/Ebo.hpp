#pragma once
#include <glad/glad.h>
#include <vector>

namespace DazaiEngine 
{
	class Ebo
	{
	public:
		Ebo(std::vector<GLuint> indices);
		auto bind() -> void;
		auto unBind() -> void;
		auto destroy() -> void;
		unsigned int id = 0;

	private:
		
	};
}