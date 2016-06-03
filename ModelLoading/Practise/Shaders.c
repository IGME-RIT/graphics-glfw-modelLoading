#include "Shaders.h"

#include <stdio.h>
#include <stdlib.h>

GLuint Shaders_CreateShaderProgram(struct ShaderPair *shader_pair, GLint num_shaders)
{
	GLuint *shaders = (GLuint *)malloc(num_shaders * sizeof(GLuint));
	int i;
	for (i = 0; i < num_shaders; ++i)
		shaders[i] = Shaders_CreateShader(shader_pair[i].shader_source, shader_pair[i].shader_type);

	GLuint shader_program = glCreateProgram();
	for (i = 0; i < num_shaders; ++i)
		glAttachShader(shader_program, shaders[i]);
	glLinkProgram(shader_program);

	GLint status;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	printf("Compute Shader link status %d\n", status);

	GLchar error_log[512];
	glGetProgramInfoLog(shader_program, 512, NULL, error_log);
	printf("Compute Program error log: %s\n", error_log);

	for (i = 0; i < num_shaders; ++i)
		glDeleteShader(shaders[i]);
	free(shaders);

	return shader_program;
}

GLuint Shaders_CreateShader(GLchar const *shader_source, GLenum shader_type)
{
	FILE *fptr;
	GLuint length;
	GLchar *buffer;

	fptr = fopen(shader_source, "rb");

	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);

	buffer = (GLchar *)malloc((length + 1) * sizeof(GLchar));
	fseek(fptr, 0, SEEK_SET);
	fread(buffer, length, sizeof(GLchar), fptr);
	buffer[length] = 0;

	fclose(fptr);

	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &buffer, NULL);
	glCompileShader(shader);

	free(buffer);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	printf("Shader compile status %d\n", status);

	GLchar error_log[512];
	glGetShaderInfoLog(shader, 512, NULL, error_log);
	printf("Shader error log: %s\n", error_log);

	return shader;
}