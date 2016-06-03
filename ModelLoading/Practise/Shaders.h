#ifndef __Shaders__
#define __Shaders__

#include "Types.h"

GLuint Shaders_CreateShaderProgram(struct ShaderPair *shader_pair, GLint num_shaders);
GLuint Shaders_CreateShader(GLchar const *shader_source, GLenum shader_type);

#endif