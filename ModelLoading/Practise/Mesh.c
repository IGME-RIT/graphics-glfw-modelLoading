#include "Mesh.h"

#include <stddef.h>
#include <stdio.h>

void Mesh_Setup(struct Mesh *mesh)
{
	glGenVertexArrays(1, &mesh->vao);
	glGenBuffers(1, &mesh->vbo);
	glGenBuffers(1, &mesh->ebo);

	glBindVertexArray(mesh->vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
		{
			glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * sizeof(struct Vertex), mesh->vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid *)offsetof(struct Vertex, position));
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid *)offsetof(struct Vertex, normal));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid *)offsetof(struct Vertex, texcoords));
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid *)offsetof(struct Vertex, tangent));
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (GLvoid *)offsetof(struct Vertex, bitangent));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			glEnableVertexAttribArray(4);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_count * sizeof(GLuint), mesh->indices, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
}

void Mesh_Render(struct Mesh *mesh, GLuint shader_program)
{
	/*
		It is easy to render the vertex data of the mesh by simply using drawelements below. However, to get a texture rendered we need to bind it, and use the sampler in the shader. 
		Depending on what type the texture is, we bind it and pass it as a uniform.
	*/
	GLuint diffuse_no = 1;
	GLuint specular_no = 1;
	GLuint normal_no = 1;
	GLuint height_no = 1;
	GLuint i;
	for (i = 0; i < mesh->texture_count; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		
		GLchar name[25];
		strcpy(name, mesh->textures[i].type);
		GLchar number[3];
		if (strcmp(name, "texture_diffuse") == 0)
			sprintf(number, "%d", diffuse_no++);
		else if (strcmp(name, "texture_specular") == 0)
			sprintf(number, "%d", specular_no++);
		else if (strcmp(name, "texture_normal") == 0)
			sprintf(number, "%d", normal_no++);
		else if (strcmp(name, "texture_height") == 0)
			sprintf(number, "%d", height_no++);
		strcat(name, number);

		glUniform1i(glGetUniformLocation(shader_program, name), i);
		glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
	}

	glBindVertexArray(mesh->vao);
	{
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	for (i = 0; i < mesh->texture_count; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}