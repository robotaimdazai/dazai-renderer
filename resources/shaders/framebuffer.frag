#version 330 core

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

uniform float gamma = 2.2f;
uniform float exposure =2f;
uniform float bloomIntensity =0.5f;

uniform bool useHdr = true;
uniform bool useBloom = true;


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
    vec3 bloom =    texture(bloomTexture, texCoords).rgb;
    vec3 color = fragment;
    if(useBloom)
    {
        color = fragment + bloom * bloomIntensity;
    } 

    if(useHdr)
	{
		color = vec3(1.0f) - exp(-color * exposure); //tonemapped
	}
    FragColor.rgb = pow(color, vec3(1.0f/gamma));
}