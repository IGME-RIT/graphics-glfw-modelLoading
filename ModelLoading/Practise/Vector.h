#ifndef __Vector__
#define __Vector__

#include "Types.h"

#include <math.h>

static _inline void vec2_Zero(vec2 v)
{
	GLuint i;
	for (i = 0; i < 2; ++i)
		v[i] = 0.0f;
}

static _inline void vec2_Copy(vec2 destination, vec2 source)
{
	GLuint i;
	for (i = 0; i < 2; ++i)
		destination[i] = source[i];
}

static _inline void vec3_Zero(vec3 v)
{
	GLuint i;
	for (i = 0; i < 3; ++i)
		v[i] = 0.0f;
}

static _inline void vec3_Scale(vec3 res, vec3 v, GLfloat s)
{
	GLuint i;
	for (i = 0; i < 3; ++i)
		res[i] = v[i] * s;
}

static _inline void vec3_Add(vec3 res, vec3 a, vec3 b)
{
	GLuint i;
	for (i = 0; i < 3; ++i)
		res[i] = a[i] + b[i];
}

static _inline void vec3_Sub(vec3 res, vec3 a, vec3 b)
{
	GLuint i;
	for (i = 0; i < 3; ++i)
		res[i] = a[i] - b[i];
}

static _inline void vec3_Norm(vec3 res, vec3 v)
{
	GLuint i;
	GLfloat mag = 0.0f;
	for (i = 0; i < 3; ++i)
		mag += v[i] * v[i];
	sqrt(mag);
	vec3_Scale(res, v, 1.0f / mag);
}

static _inline void vec3_Negate(vec3 res, vec3 v)
{
	GLuint i;
	for (i = 0; i < 3; ++i)
		res[i] = -v[i];
}

static _inline void vec3_Mul(vec3 res, vec3 a, vec3 b)
{
	GLuint i;
	for (i = 0; i < 3; ++i)
		res[i] = a[i] * b[i];
}

static _inline GLfloat vec3_DotProd(vec3 a, vec3 b)
{
	GLfloat prod = 0.0f;
	GLuint i = 0;
	for (i = 0; i < 3; ++i)
		prod += a[i] * b[i];

	return prod;
}

static _inline void vec3_CrossProd(vec3 res, vec3 a, vec3 b)
{
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

static _inline void vec3_Copy(vec3 destination, vec3 source)
{
	GLuint i;
	for (i = 0; i < 3; ++i)
		destination[i] = source[i];
}

static _inline void vec4_Scale(vec3 res, vec4 v, GLfloat s)
{
	GLuint i;
	for (i = 0; i < 4; ++i)
		res[i] = v[i] * s;
}

static _inline vec4_Copy(vec4 destination, vec4 source)
{
	GLuint i;
	for (i = 0; i < 4; ++i)
		destination[i] = source[i];
}

static _inline void vec4_Add(vec4 res, vec4 a, vec4 b)
{
	GLuint i;
	for (i = 0; i < 4; ++i)
		res[i] = a[i] + b[i];
}

static _inline GLfloat vec4_DotProd(vec4 a, vec4 b)
{
	GLfloat prod = 0.0f;
	GLuint i = 0;
	for (i = 0; i < 4; ++i)
		prod += a[i] * b[i];

	return prod;
}

static _inline void vec4_MulMat4x4(vec4 res, vec4 vec, mat4x4 M)
{
	GLuint i, j;
	GLfloat sum = 0.0f;

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			sum += vec[j] * M[j][i];
		}

		res[i] = sum;
		sum = 0.0f;
	}
}

static _inline void mat3x3_MulVec3(vec3 res, mat4x4 M, vec3 vec)
{
	GLuint i, j;
	GLfloat sum = 0.0f;

	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			sum += M[i][j] * vec[j];
		}

		res[i] = sum;
		sum = 0.0f;
	}
}

static _inline void mat4x4_Identity(mat4x4 M)
{
	GLuint i, j;
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 4; ++j)
			M[i][j] = (i == j) ? 1.0f : 0.0f;
}

static _inline void mat4x4_Transpose(mat4x4 M, mat4x4 N)
{
	GLuint i, j;
	for (j = 0; j<4; ++j)
		for (i = 0; i<4; ++i)
			M[i][j] = N[j][i];
}

static _inline void mat4x4_Mul(mat4x4 R, mat4x4 A, mat4x4 B)
{
	GLuint i, j, k;
	float sum = 0.0f;

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			for (k = 0; k < 4; ++k)
			{
				sum += A[i][k] * B[k][j];
			}

			R[i][j] = sum;
			sum = 0.0f;
		}
	}
}

static _inline void mat4x4_Trans(mat4x4 T, vec3 v)
{
	mat4x4_Identity(T);

	GLuint i;
	for (i = 0; i < 3; ++i)
		T[i][3] = v[i];
}

static _inline void mat4x4_Scale(mat4x4 S, vec3 v)
{
	mat4x4_Identity(S);

	GLuint i;
	for (i = 0; i < 3; ++i)
		S[i][i] *= v[i];
}

static _inline void quat_Identity(quat q)
{
	GLuint i;
	for (i = 0; i < 3; ++i)
		q[i] = 0.0f;
	q[3] = 1.0f;
}

static _inline void quat_FromAxisAngle(quat q, vec3 axis, GLfloat angle)
{
	float half_angle = angle * 0.5f * PI / 180;
	float sin_angle = (GLfloat)sin(half_angle);
	float cos_angle = (GLfloat)cos(half_angle);

	q[0] = axis[0] * sin_angle;
	q[1] = axis[1] * sin_angle;
	q[2] = axis[2] * sin_angle;
	q[3] = cos_angle;
}

static _inline void quat_Norm(quat res, quat v)
{
	GLuint i;
	GLfloat mag = 0.0f;
	for (i = 0; i < 4; ++i)
		mag += v[i] * v[i];
	sqrtf(mag);

	if (mag == 0.0f)
	{
		res[0] = 0.0f;
		res[1] = 0.0f;
		res[2] = 0.0f;
		res[3] = 1.0f;
	}

	else
		vec4_Scale(res, v, 1.0f / mag);
}

static _inline void quat_Mul(quat r, quat a, quat b)
{
	r[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
	r[1] = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
	r[2] = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];
	r[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];
}

static _inline void mat4x4_Rot(mat4x4 R, quat q)
{
	mat4x4 A =
	{
		q[3], -q[2], q[1], -q[0],
		q[2], q[3], -q[0], -q[1],
		-q[1], q[0], q[3], -q[2],
		q[0], q[1], q[2], q[3]
	};

	mat4x4 B =
	{
		q[3], -q[2], q[1], q[0],
		q[2], q[3], -q[0], q[1],
		-q[1], q[0], q[3], q[2],
		-q[0], -q[1], -q[2], q[3]
	};

	mat4x4_Mul(R, A, B);
}

#endif
