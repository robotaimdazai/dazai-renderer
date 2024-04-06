#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;
layout (location = 4) in mat4 aInstanceMatrix;

out DATA
{
	vec3 normal;
	vec3 color;
	vec2 texCoord;
	mat4 projection;
}data_out;

uniform mat4 camMatrix;

void main()
{
	
	gl_Position = aInstanceMatrix * vec4(aPos,1.0f);
	data_out.normal = aNormal;
	data_out.color = aColor;
	data_out.texCoord = aTex;
	data_out.normal = aNormal;
	data_out.projection = camMatrix;
};