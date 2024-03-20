#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 currentPos;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float ambient = 0.1f;
uniform float specularLight =0.5f;
uniform float specPower = 1;

void main()
{
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currentPos);
	float diffuse = max(dot(normalizedNormal,lightDirection),0.0f);
	vec3 viewDirection = normalize(camPos- currentPos);
	vec3 reflectionDirection = reflect(-lightDirection,normalizedNormal);
	float specAmount = pow(max(dot(viewDirection,reflectionDirection),0.0f),specPower);
	float specular = specAmount * specularLight;
	//final
	FragColor = (texture(tex0, texCoord) * (diffuse + ambient) + texture(tex1, texCoord).r * specular) * lightColor;
};