#version 330 core

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float gamma = 2.2f;
uniform int useHdr =1;
uniform float exposure =2f;

//for neighbor pixels
const float offset_x = 1.0f / 800.0f;  
const float offset_y = 1.0f / 800.0f;  

vec2 offsets[9] = vec2[]
(
    vec2(-offset_x,  offset_y), vec2( 0.0f,    offset_y), vec2( offset_x,  offset_y),
    vec2(-offset_x,  0.0f),     vec2( 0.0f,    0.0f),     vec2( offset_x,  0.0f),
    vec2(-offset_x, -offset_y), vec2( 0.0f,   -offset_y), vec2( offset_x, -offset_y) 
);

//matrix used for sampling behavior
float kernel[9] = float[]
(
    1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

void main()
{
    /*
    vec3 color = vec3(0.0f);
    for(int i = 0; i < 9; i++)
    {
        color += vec3(texture(screenTexture, texCoords + offsets[i])) * kernel[i];
    }

    FragColor = vec4(color,1.0f);
    */

    vec3 fragment = texture(screenTexture, texCoords).rgb;
    if(useHdr == 1)
	{
		fragment = vec3(1.0f) - exp(-fragment * exposure); //tonemapped
	}
    FragColor.rgb = pow(fragment, vec3(1.0f/gamma));
}