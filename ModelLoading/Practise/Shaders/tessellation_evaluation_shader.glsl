#version 440 core

layout (triangles, equal_spacing, cw) in;

in vec4 tcs_color[];

out vec4 tes_color;

void main()
{ 
	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + (gl_TessCoord.y * gl_in[1].gl_Position) + (gl_TessCoord.z * gl_in[2].gl_Position);
	tes_color = (gl_TessCoord.x * tcs_color[0]) + (gl_TessCoord.y * tcs_color[1]) + (gl_TessCoord.z * tcs_color[2]);
}          