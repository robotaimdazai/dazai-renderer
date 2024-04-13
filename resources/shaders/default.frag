#version 330 core

out vec4 FragColor;

in vec3 currentPos;
in vec2 texCoord;
in vec3 normal;
in vec3 color;
in vec3 camPos;
in vec4 fragPosLight;
in vec3 vLightPos;
in vec3 tbnCamPos;
in vec3 tbnLightPos;
in vec3 tbnFragPos;
								//slots
uniform sampler2D diffuse0;		//0
uniform sampler2D specular0;	//1
uniform sampler2D normal0;		//2
uniform sampler2D ao0;			//3
uniform sampler2D shadowMap;	//na

uniform vec4 lightColor;
uniform float ambient = 0.3f;
uniform float specularLight =1.0f;
uniform float specPower = 128;
uniform int blinnPhong = 1;
uniform int shadowMapping =	0;
uniform int useNormalMap = 1;
uniform int useAmbientOcclusion = 1;



vec4 pointLight()
{
	vec3 ligtVec = vLightPos - currentPos;
	float d = length(ligtVec);
	float a = 3.0;
	float b = 0.7;
	float attenuation = 1.0f/(a * d * d) + (b * d + 1f);
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDirection = normalize(ligtVec);
	if(useNormalMap == 1)
	{
		vec3 normalMap = texture(normal0, texCoord).rgb;
		normalizedNormal = normalize(normalMap * 2.0 - 1.0);
		lightDirection = normalize(tbnFragPos - lightDirection);
	}
	float diffuse = max(dot(normalizedNormal,lightDirection),0.0f);
	float specular = 0.0f;
	if(diffuse!=0.0f)//to prevent from light under the object
	{
		vec3 viewDirection = normalize(camPos- currentPos);
		if(useNormalMap == 1)
		{
			viewDirection= normalize(tbnCamPos - tbnFragPos);
		}
		vec3 reflectionDirection = reflect(-lightDirection,normalizedNormal);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		if(blinnPhong ==1)
		{
			reflectionDirection = halfwayVec;
		}
		float specAmount = pow(max(dot(normalizedNormal,halfwayVec),0.0f),specPower);
		specular = specAmount * specularLight;
	}

	float ao = 1;
	if(useAmbientOcclusion==1)
	{
		ao = texture(ao0, texCoord).r; // Assuming AO map is greyscale
	}
	//final
	vec4 diffuseColor = texture(diffuse0, texCoord);
	float specularIntensity = texture(specular0, texCoord).r;

	// Calculate diffuse and ambient contributions
	vec3 diffuseComponent = diffuseColor.rgb * diffuse  * attenuation * ao;
	vec3 ambientComponent = diffuseColor.rgb * ambient * ao; // Assuming ambient uses the same texture
	// Calculate specular contribution
	vec3 specularComponent = vec3(specularIntensity) * specular * attenuation;
	// Combine all lighting components
	vec3 finalColor = (diffuseComponent + ambientComponent + specularComponent) * lightColor.rgb;
	// Return the final color with full opacity
	return vec4(finalColor, 1.0);
	

}

vec4 directionalLight()
{
	vec3 lightDirection = normalize(vLightPos);
	vec3 normalizedNormal = normalize(normal);
	if(useNormalMap == 1)
	{
		vec3 normalMap = texture(normal0, texCoord).rgb;
		normalizedNormal = normalize(normalMap * 2.0 - 1.0);
		lightDirection = normalize(tbnFragPos - lightDirection);
	}
	float diffuse = max(dot(normalizedNormal,lightDirection),0.0f);
	float specular = 0.0f;
	if(diffuse!=0.0f)
	{
		vec3 viewDirection = normalize(camPos - currentPos);
		if(useNormalMap == 1)
		{
			viewDirection= normalize(tbnCamPos - tbnFragPos);
		}
		vec3 reflectionDirection = reflect(-lightDirection,normalizedNormal);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		if(blinnPhong ==1)
		{
			reflectionDirection = halfwayVec;
		}
		float specAmount = pow(max(dot(normalizedNormal,reflectionDirection),0.0f),specPower);
		specular = specAmount * specularLight;
	}
	//shadowmap
	float shadow = 0.0f;
	vec3 lightCooords = fragPosLight.xyz/fragPosLight.w;
	if(lightCooords.z <= 1.0f) // far plane max = 1.0f
	{
		lightCooords = (lightCooords + 1.0f)/2.0f; // change it to 0-1 range as depth buffer is in same range
		float shadowMapDepth = texture(shadowMap, lightCooords.xy).r;
		float currentDepth = lightCooords.z;
		float bias = 0.005f;
		if(currentDepth > shadowMapDepth + bias && shadowMapping == 1)
		{
			shadow = 1.0f;
		}
	}

	float ao = 1;
	if(useAmbientOcclusion==1)
	{
		ao = texture(ao0, texCoord).r; // Assuming AO map is greyscale
	}
	//final
	vec4 diffuseColor = texture(diffuse0, texCoord);
	float specularIntensity = texture(specular0, texCoord).r;

	// Calculate diffuse and ambient contributions
	vec3 diffuseComponent = diffuseColor.rgb * diffuse * (1.0f - shadow) * ao ;
	vec3 ambientComponent = diffuseColor.rgb * ambient * ao; // Assuming ambient uses the same texture

	// Calculate specular contribution
	vec3 specularComponent = vec3(specularIntensity) * specular * (1.0f - shadow) * ao;

	// Combine all lighting components
	vec3 finalColor = (diffuseComponent + ambientComponent + specularComponent) * 
						lightColor.rgb;
	// Return the final color with full opacity
	return vec4(finalColor, 1.0);
}


vec4 spotLight()
{
	float outerCone = 0.1f;
	float innerCone = 0.2f;
	vec3 ligtVec = vLightPos - currentPos;
	vec3 lightDirection = normalize(ligtVec);
	vec3 normalizedNormal = normalize(normal);
	if(useNormalMap == 1)
	{
		vec3 normalMap = texture(normal0, texCoord).rgb;
		normalizedNormal = normalize(normalMap * 2.0 - 1.0);
		lightDirection = normalize(tbnFragPos - vLightPos);
	}
	float diffuse = max(dot(normalizedNormal,lightDirection),0.0f);
	float specular = 0.0f;
	float intensity = 0.0f;
	if(diffuse!=0.0f)
	{
		vec3 viewDirection = normalize(camPos- currentPos);
		if(useNormalMap == 1)
		{
			viewDirection= normalize(tbnCamPos - tbnFragPos);
		}
		vec3 reflectionDirection = reflect(-lightDirection,normalizedNormal);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		if(blinnPhong ==1)
		{
			reflectionDirection = halfwayVec;
		}
		float specAmount = pow(max(dot(normalizedNormal,halfwayVec),0.0f),specPower);
		specular = specAmount * specularLight;
		
	}

	float angle = dot(vec3(0.0f,-1.0f,0.0f), - lightDirection);
	intensity = clamp((angle - outerCone)/(innerCone-outerCone),0.0f,1.0f);
	
	float ao = 1;
	if(useAmbientOcclusion==1)
	{
		ao = texture(ao0, texCoord).r; // Assuming AO map is greyscale
	}
	//final
	vec4 diffuseColor = texture(diffuse0, texCoord);
	float specularIntensity = texture(specular0, texCoord).r;

	// Calculate diffuse and ambient contributions
	vec3 diffuseComponent = diffuseColor.rgb * diffuse * intensity;
	vec3 ambientComponent = diffuseColor.rgb * ambient * ao ; // Assuming ambient uses the same texture

	// Calculate specular contribution
	vec3 specularComponent = vec3(specularIntensity) * specular  * intensity;

	// Combine all lighting components
	vec3 finalColor = (diffuseComponent + ambientComponent + specularComponent) * lightColor.rgb;
	
	// Return the final color with full opacity
	return vec4(finalColor, 1.0);
	
}

float near = 0.1f;
float far = 100.0f;

float linearizeDepth(float depth)
{
	return(2.0 * near * far)/(far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth, float steepness=0.5f, float offset =5.0f)
{
	float zVal = linearizeDepth(depth);
	return ( 1 / ( 1 + exp(-steepness * (zVal - offset))) );

}
void main()
{
	//for fog
	//FragColor = directionalLight();
	//float depth  = logisticDepth(gl_FragCoord.z);
	//FragColor = directionalLight() * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.90f), 1.0f);
	//----
	FragColor = directionalLight();
};