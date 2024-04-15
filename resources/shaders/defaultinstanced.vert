#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in mat4 aInstanceMatrix;

out vec3 normal;
out vec3 color;
out vec2 texCoord;
out vec3 currentPos;
out vec3 vcamPos;
out vec3 vLightPos;

uniform mat4 camMatrix;
uniform vec3 camPos;
uniform vec3 lightPos;

void main()
{
	currentPos = vec3( aInstanceMatrix * vec4(aPos,1.0f));
	vLightPos = lightPos;
	normal = aNormal;
	color = aColor;
	texCoord = aTex;
	vcamPos = camPos;
	gl_Position = camMatrix * vec4(currentPos,1.0f);

};