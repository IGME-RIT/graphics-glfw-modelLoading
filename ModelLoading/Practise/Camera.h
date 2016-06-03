#ifndef __Camera__
#define __Camera__

#include "Vector.h"
#include "Transform.h"

typedef struct Camera
{
	Transform transform;
	mat4x4 view_matrix;

	float fov;
	float aspect_ratio;
	float near;
	float far;
	mat4x4 projection_matrix;
} Camera;

Camera *Camera_Init();
void Camera_GetViewMat(Camera *camera);
void Camera_GetProjMat(Camera *camera);
void Camera_Render(Camera *camera, GLuint shader_prog);
void Camera_Destroy(Camera *camera);

#endif
