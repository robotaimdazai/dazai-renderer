#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices =3) out;

out vec3 currentPos;
out vec2 texCoord;
out vec3 normal;
out vec3 color;
out vec4 fragPosLight;

in DATA
{
	vec3 normal;
	vec3 color;
	vec2 texCoord;
	mat4 projection;
	mat4 lightProjection;
}data_in[];

void main()
{
	gl_Position = data_in[0].projection * gl_in[0].gl_Position;
	currentPos = gl_in[0].gl_Position.xyz;
	normal = data_in[0].normal;
	color = data_in[0].color;
	texCoord = data_in[0].texCoord;
	fragPosLight = data_in[0].lightProjection * gl_in[0].gl_Position;
	EmitVertex();

	gl_Position = data_in[1].projection * gl_in[1].gl_Position;
	currentPos = gl_in[1].gl_Position.xyz;
	normal = data_in[1].normal;
	color = data_in[1].color;
	texCoord = data_in[1].texCoord;
	fragPosLight = data_in[1].lightProjection * gl_in[1].gl_Position;
	EmitVertex();

	gl_Position = data_in[2].projection * gl_in[2].gl_Position;
	currentPos = gl_in[2].gl_Position.xyz;
	normal = data_in[2].normal;
	color = data_in[2].color;
	texCoord = data_in[2].texCoord;
	fragPosLight = data_in[2].lightProjection * gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}