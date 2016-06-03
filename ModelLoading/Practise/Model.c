#include "Model.h"
#include "Mesh.h"

#include <assimp/postprocess.h>
#include <SOIL/SOIL.h>

#include <stdlib.h>
#include <string.h>

struct Model *Model_Load(GLchar const *model_source)
{
	struct Model *model = (struct Model *)malloc(sizeof(struct Model));
	model->mesh_count = 0;
	model->meshes = NULL;
	model->textures_loaded = NULL;
	model->textures_loaded_count = 0;

	/*
		Every model loaded using assimp is stored in somthing called a scene variable. 
		The scene variable consists of all the data which opengl needs to know, to be able to render that model.
		For every model, the function aiImportFile will return one scene variable consisting of the meshes in the current scene, the materials of the current scene and child nodes.
		Each child nodel further has the same information: meshes, materials and child nodes if any. Collective this heirarchy makes one model.

		Hence we recursively iterate through all the nodes of the scene, collecting and storing the mesh data like vertex position, index positions into our Mesh struct variable and material data like textures into our texture struct variable.
		How we do this will be explained below as we go through every step. For now, in the line below, all the data we need is in the assimp aiScene struct variable.
	*/
	struct aiScene const *scene = aiImportFile(model_source, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	char *directory_length = strrchr(model_source, '/') + 1;

	GLuint i;
	for (i = 0; model_source + i != directory_length; ++i)
		model->directory[i] = model_source[i];
	model->directory[i] = 0;

	/*
		Start of recursive call to transfer assimp scene data into our model struct. 
	*/
	Model_ProcessNode(model, scene->mRootNode, scene);

	return model;
}

void Model_ProcessNode(struct Model *model, struct aiNode *node, struct aiScene const *scene)
{
	/*
		Each model will have 'n' number of meshes depending on the model we are loading. Assimp follows a tree structure to store mesh data.
		We however don't, and everything is in one mesh array for every model. Hence, we re-allocate memory for everytime this function is called recursively and new meshes are added from child nodes.
		This problem can be simply avoided if a vector container is used in c++. However it does the same thing more efficiently.
	*/

	model->meshes = (struct Mesh*)realloc(model->meshes, (model->mesh_count + node->mNumMeshes) * sizeof(struct Mesh));

	GLuint i;
	for (i = 0; i < node->mNumMeshes; ++i)
	{
		/*
			Now that each node is being processed for its meshes, each mesh needs to be processed for its veretx and index data. That data is stored in assimp's aiMesh struct type variable.
			To read that structure, we call the Model_ProcessMesh function.
		*/
		struct aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
		Model_ProcessMesh(model, ai_mesh, scene);
	}

	for (i = 0; i < node->mNumChildren; ++i)
	{
		/*
			recursive call to process each child node in scene.
		*/
		Model_ProcessNode(model, node->mChildren[i], scene);
	}
}

void Model_ProcessMesh(struct Model *model, struct aiMesh *ai_mesh, struct aiScene const *scene)
{
	/*
		Each Mesh consists of a Vertex, index and texture structure. See Types.h for declerations.
		We need transfer data from assimp's aiMesh to our mesh struct. However, these structres don't match up exactly and we need to manually transfer the data. 
	*/

	struct Mesh *current_mesh = &model->meshes[model->mesh_count];
	
	current_mesh->vertex_count = ai_mesh->mNumVertices;
	current_mesh->vertices = (struct Vertex *)malloc(current_mesh->vertex_count * sizeof(struct Vertex));

	GLuint i;
	for (i = 0; i < current_mesh->vertex_count; ++i)
	{
		/*
			Transfer of vertex position data from assimp aiMesh to our vertex.position.
			Similar actions performed for normals, texture coordinates if any, tangents and bitangents.
		*/
		current_mesh->vertices[i].position[0] = ai_mesh->mVertices[i].x;
		current_mesh->vertices[i].position[1] = ai_mesh->mVertices[i].y;
		current_mesh->vertices[i].position[2] = ai_mesh->mVertices[i].z;
	
		current_mesh->vertices[i].normal[0] = ai_mesh->mNormals[i].x;
		current_mesh->vertices[i].normal[1] = ai_mesh->mNormals[i].y;
		current_mesh->vertices[i].normal[2] = ai_mesh->mNormals[i].z;

		if (ai_mesh->mTextureCoords[0])
		{
			current_mesh->vertices[i].texcoords[0] = ai_mesh->mTextureCoords[0][i].x;
			current_mesh->vertices[i].texcoords[1] = ai_mesh->mTextureCoords[0][i].y;
		}

		else
		{
			current_mesh->vertices[i].texcoords[0] = 0.0f;
			current_mesh->vertices[i].texcoords[1] = 0.0f;
		}

		current_mesh->vertices[i].tangent[0] = ai_mesh->mTangents[i].x;
		current_mesh->vertices[i].tangent[1] = ai_mesh->mTangents[i].y;
		current_mesh->vertices[i].tangent[2] = ai_mesh->mTangents[i].z;

		current_mesh->vertices[i].bitangent[0] = ai_mesh->mBitangents[i].x;
		current_mesh->vertices[i].bitangent[1] = ai_mesh->mBitangents[i].y;
		current_mesh->vertices[i].bitangent[2] = ai_mesh->mBitangents[i].z;
	}

	/*
		Once we have the vertex data, we need the index data because in large models, it's always efficient to draw using indices.
		The assimp aiMesh structure consists of something called mFaces, which inturn constists of the index data of the object.
		Hence we have to iterate through all the faces for all the indices in them and store them in our Mesh->indices array.
		Again, reallocation can be avoided using vectors in c++.
	*/
	current_mesh->indices = NULL;
	current_mesh->index_count = 0;
	GLuint current_index_count = 0;
	for (i = 0; i < ai_mesh->mNumFaces; ++i)
	{
		struct aiFace face = ai_mesh->mFaces[i];
		current_mesh->index_count += face.mNumIndices;
		current_mesh->indices = (GLuint *)realloc(current_mesh->indices, current_mesh->index_count * sizeof(GLuint));
		GLuint j;
		for (j = 0; current_index_count < current_mesh->index_count; ++current_index_count, ++j)
			current_mesh->indices[current_index_count] = face.mIndices[j];
	}

	/*
		We now come to our final step of loading textures from Material data. Material data lies in the assimp aiMaterial variable. Each material consists of textures of different types.
		We can choose to get whatever types of textures we want and ignore the rest.
	*/
	current_mesh->textures = NULL;
	current_mesh->texture_count = 0;
	if (ai_mesh->mMaterialIndex >= 0)
	{
		struct aiMaterial *material = scene->mMaterials[ai_mesh->mMaterialIndex];

		GLuint texture_counts[4] = { 0 };
		texture_counts[0] = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
		texture_counts[1] = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);
		texture_counts[2] = aiGetMaterialTextureCount(material, aiTextureType_HEIGHT);
		texture_counts[3] = aiGetMaterialTextureCount(material, aiTextureType_AMBIENT);
		current_mesh->texture_count = texture_counts[0] + texture_counts[1] + texture_counts[2] + texture_counts[3];

		current_mesh->textures = (struct Texture *)malloc((current_mesh->texture_count)* sizeof(struct Texture));
		/*
			Loading textures of different types into the meshe's textures variable. See Type.h for texture decleration.
		*/
		Model_LoadMaterialTextures(model, current_mesh->textures, 0, texture_counts[0], material, aiTextureType_DIFFUSE, "texture_diffuse");
		Model_LoadMaterialTextures(model, current_mesh->textures, texture_counts[0], texture_counts[0] + texture_counts[1], material, aiTextureType_SPECULAR, "texture_specular");
		Model_LoadMaterialTextures(model, current_mesh->textures, texture_counts[0] + texture_counts[1], texture_counts[0] + texture_counts[1] + texture_counts[2], material, aiTextureType_HEIGHT, "texture_normal");
		Model_LoadMaterialTextures(model, current_mesh->textures, texture_counts[0] + texture_counts[1] + texture_counts[2], current_mesh->texture_count, material, aiTextureType_AMBIENT, "texture_height");
	}

	/*
		Once we have the data, we need to send it over to the GPU. This happens in the mesh setup function.
	*/
	Mesh_Setup(current_mesh);
	model->mesh_count++;
}

void Model_LoadMaterialTextures(struct Model *model, struct Texture *textures, GLuint start_index, GLuint end_index, struct aiMaterial *material, enum aiTextureType type, GLchar const *type_name)
{
	GLuint i;
	GLuint j;
	GLuint k = start_index;
	GLuint texture_type_count = end_index - start_index;
	for (i = 0; i < texture_type_count; ++i)
	{
		struct aiString string;
		/*
			The assimp function call to get a particular type of texture. The result is the texture file name stored in assimp's string data type.
			We then need to load the texture onto the gpu and that happens in the next function. See simple texturing example tutorial.
		*/
		aiGetMaterialTexture(material, type, i, &string, NULL, NULL, NULL, NULL, NULL, NULL);

		GLboolean skip = 0;
		for (j = 0; j < model->textures_loaded_count; ++j)
		{
			if (strcmp(model->textures_loaded[j].path.data, string.data) == 0)
			{
				textures[k++] = model->textures_loaded[j];
				skip = 1;
				break;
			}
		}

		if (!skip)
		{
			textures[k].id = Model_TextureFromFile(string.data, model->directory, 0);
			strcpy(textures[k].type, type_name);
			textures[k].path = string;
			model->textures_loaded = (struct Texture *)realloc(model->textures_loaded, (model->textures_loaded_count + 1) * sizeof(struct Texture));
			model->textures_loaded[model->textures_loaded_count++] = textures[k++];
		}
	}
}

GLint Model_TextureFromFile(const GLchar* path, GLchar const *directory, GLboolean gamma)
{
	GLchar filename[100];
	strcpy(filename, directory);
	strcat(filename, path);

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	GLint width, height;
	unsigned char *image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, gamma ? GL_SRGB : GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return texture_id;
}

void Model_Render(struct Model *model, GLuint shader_prog)
{
	/*
		Every mesh of the model is rendered.
	*/
	GLuint i;
	for (i = 0; i < model->mesh_count; ++i)
	{
		Mesh_Render(&model->meshes[i], shader_prog);
	}
}