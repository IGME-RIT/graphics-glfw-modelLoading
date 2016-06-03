#ifndef __Types__
#define __Types__

#include <GL/glew.h>
#include <assimp/cimport.h>

#define PI 3.141592f

typedef GLfloat vec2[2];
typedef GLfloat vec3[3];
typedef GLfloat vec4[4];
typedef vec4 mat4x4[4];
typedef GLfloat quat[4];

struct Vertex
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texcoords[2];
	GLfloat tangent[3];
	GLfloat bitangent[3];
};

struct Texture
{
	GLuint id;
	GLchar type[25];
	struct aiString path;
};

struct Mesh
{
	struct Vertex *vertices;
	GLuint *indices;
	struct Texture *textures;
	GLuint vertex_count;
	GLuint index_count;
	GLuint texture_count;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

struct Model
{
	struct Texture *textures_loaded;
	GLuint textures_loaded_count;
	struct Mesh *meshes;
	GLuint mesh_count;
	GLchar directory[50];
};

struct ShaderPair
{
	GLchar const *shader_source;
	GLenum shader_type;
};

//struct Camera
//{
//	GLfloat eye[3];
//	GLfloat look[3];
//	GLfloat up[3];
//};

#endif