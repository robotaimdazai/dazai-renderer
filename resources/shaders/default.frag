#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform float ambientLight = 0.2f;

void main()
{
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currentPos);
	float diffuse = max(dot(normalizedNormal,lightDirection),0.0f);
	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambientLight);
};