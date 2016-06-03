#version 440 core

layout (local_size_x = 3, local_size_y = 1, local_size_z = 1) in;

uniform float time;

layout(std430, binding = 0) buffer pos
{
	vec4 position[];
};

void main()
{
		uint idx = gl_GlobalInvocationID.x;

		position[idx].x = position[idx].x + (sin(time) / 100);
}