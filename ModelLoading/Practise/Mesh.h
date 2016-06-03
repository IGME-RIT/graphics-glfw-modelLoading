#ifndef __Mesh__
#define __Mesh__

#include "Types.h"

void Mesh_Setup(struct Mesh *mesh);
void Mesh_Render(struct Mesh *mesh, GLuint shader_program);

#endif