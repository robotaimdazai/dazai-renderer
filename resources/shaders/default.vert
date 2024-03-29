#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;



out vec3 color;
out vec2 texCoord;
out vec3 currentPos;
out vec3 normal;

uniform mat4 camMatrix;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
void main()
{
	currentPos = vec3(translation * rotation * scale * vec4(aPos, 1.0f));
	gl_Position = camMatrix * vec4(currentPos,1.0f);
	color = aColor;
	texCoord = aTex;
	normal = aNormal;
};