#version 440 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 vs_normal;
out vec2 vs_texcoords;
out vec3 vs_tangent;
out vec3 vs_bitangent;

uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	gl_Position = u_proj * u_view * vec4(position, 1.0f);
	vs_normal = normal;
	vs_texcoords = texcoords;
	vs_tangent = tangent;
	vs_bitangent = bitangent;
}