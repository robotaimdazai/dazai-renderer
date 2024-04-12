#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aColor;
layout (location = 4) in vec3 aTangent;


out vec3 currentPos;
out vec2 texCoord;
out vec3 normal;
out vec3 color;
out vec3 vLightPos;
out vec4 fragPosLight;
out vec3 tbnCamPos;
out vec3 tbnLightPos;
out vec3 tbnFragPos;


uniform mat4 model;
uniform mat4 camMatrix;
uniform mat4 lightProjection;
uniform vec3 camPos;
uniform vec3 lightPos;

void main()
{
	currentPos = vec3(model * vec4(aPos,1.0f));
	normal = aNormal;
	color = aColor;
	texCoord = aTex;
	vLightPos = lightPos;
	fragPosLight = lightProjection * vec4(currentPos,1.0);
	gl_Position = camMatrix * vec4(currentPos, 1.0);
	//for normal mapping TBN
	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T, B, N));
	tbnLightPos = TBN * lightPos;
	tbnCamPos =	TBN * camPos;
	tbnFragPos =	TBN * currentPos;
};