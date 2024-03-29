#version 330 core

out vec4 FragColor;

in vec3 currentPos;
in vec2 texCoord;
in vec3 normal;
in vec3 color;



uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float ambient = 0.45f;
uniform float specularLight =0.6f;
uniform float specPower = 32;

vec4 pointLight()
{
	vec3 ligtVec = lightPos - currentPos;
	float d = length(ligtVec);
	float a = 3.0;
	float b = 0.7;
	float attenuation = 1.0f/(a * d * d) + (b * d + 1f);
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDirection = normalize(ligtVec);
	float diffuse = max(dot(normalizedNormal,lightDirection),0.0f);
	vec3 viewDirection = normalize(camPos- currentPos);
	vec3 reflectionDirection = reflect(-lightDirection,normalizedNormal);
	float specAmount = pow(max(dot(viewDirection,reflectionDirection),0.0f),specPower);
	float specular = specAmount * specularLight;
	//final
	return (texture(diffuse0, texCoord) * (diffuse* attenuation + ambient) + 
			(texture(specular0, texCoord).r * specular * attenuation) * lightColor);

}

vec4 directionalLight()
{
	vec3 lightDirection = normalize(vec3(0.0f,1.0f,0.0f));
	vec3 normalizedNormal = normalize(normal);
	float diffuse = max(dot(normalizedNormal,lightDirection),0.0f);
	vec3 viewDirection = normalize(camPos- currentPos);
	vec3 reflectionDirection = reflect(-lightDirection,normalizedNormal);
	float specAmount = pow(max(dot(viewDirection,reflectionDirection),0.0f),specPower);
	float specular = specAmount * specularLight;
	//final
	return (texture(diffuse0, texCoord) * (diffuse + ambient) + 
			(texture(specular0, texCoord).r * specular ) * lightColor);
}

vec4 spotLight()
{
	float outerCone = 0.2f;
	float innerCone = 0.3f;
	vec3 ligtVec = lightPos - currentPos;
	vec3 lightDirection = normalize(ligtVec);
	vec3 normalizedNormal = normalize(normal);
	float diffuse = max(dot(normalizedNormal,lightDirection),0.0f);
	vec3 viewDirection = normalize(camPos- currentPos);
	vec3 reflectionDirection = reflect(-lightDirection,normalizedNormal);
	float specAmount = pow(max(dot(viewDirection,reflectionDirection),0.0f),specPower);
	float specular = specAmount * specularLight;
	float angle = dot(vec3(0.0f,-1.0f,0.0f), - lightDirection);
	float intensity = clamp((angle - outerCone)/(innerCone-outerCone),0.0f,1.0f);
	//final
	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + 
			(texture(specular0, texCoord).r * specular * intensity ) * lightColor);
}

void main()
{
	FragColor = directionalLight();
};