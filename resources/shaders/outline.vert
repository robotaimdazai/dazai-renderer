#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

out vec3 currentPos;

uniform mat4 camMatrix;
uniform mat4 model;
uniform float outlining;

void main()
{
	currentPos = vec3(model * vec4(aPos + aNormal * outlining, 1.0f));
	gl_Position = camMatrix * vec4(currentPos,1.0f);

}