#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in vec4 tes_color[];

out vec4 gs_color;

void main()
{
	int i;
	for(i = 0; i < gl_in.length(); ++i)
	{
		gs_color = tes_color[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();

	for(i = 0; i < gl_in.length(); ++i)
	{
		gs_color = tes_color[i];
		gl_Position = gl_in[i].gl_Position + 1.0;
		EmitVertex();
	}

	EndPrimitive();
}