#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;


out vec3 currentPos;
out vec2 texCoord;
out vec3 normal;
out vec3 color;
out vec4 fragPosLight;


uniform mat4 camMatrix;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 lightProjection;

void main()
{
	currentPos = vec3(translation * rotation * scale * vec4(aPos,1.0f));
	normal = aNormal;
	color = aColor;
	texCoord = aTex;
	fragPosLight = lightProjection * vec4(currentPos,1.0);
	gl_Position = camMatrix * vec4(currentPos, 1.0);
	
};