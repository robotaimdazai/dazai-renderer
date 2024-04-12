#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;

out DATA
{
	vec3 normal;
	vec3 color;
	vec2 texCoord;
	mat4 projection;
	mat4 lightProjection;
}data_out;


uniform mat4 camMatrix;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 lightProjection;

void main()
{
	gl_Position = translation * rotation * scale * vec4(aPos,1.0f);
	data_out.normal = aNormal;
	data_out.color = aColor;
	data_out.texCoord = aTex;
	data_out.normal = aNormal;
	data_out.projection = camMatrix;
	data_out.lightProjection = lightProjection;
};