/*
	Simple model loading using assimp. Tutorial reference: http://learnopengl.com/#!Model-Loading/Assimp.
	This tutorial shows how to load model of almost any extension using assimp. Extension list: http://assimp.sourceforge.net/main_features_formats.html

	The tutorial will only concentrate on the files related to model loading which are model.c and mesh.c and the rest like camera, shaders and transformations can be learned from other tutorials.
*/

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SOIL/SOIL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "Types.h"
#include "Shaders.h"
#include "Model.h"
#include "Camera.h"

#define WIDTH 1024
#define HEIGHT 768

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_Window *gl_window;
	SDL_GLContext gl_context;

	gl_window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	gl_context = SDL_GL_CreateContext(gl_window);

	glewExperimental = GL_TRUE;
	glewInit();

	printf("Using opengl version %s.\n", glGetString(GL_VERSION));
	printf("Using glsl version %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	struct ShaderPair nanosuit_pair[2] = { [0].shader_source = "Shaders/vertex_shader.glsl", [0].shader_type = GL_VERTEX_SHADER,
		[1].shader_source = "Shaders/fragment_shader.glsl", [1].shader_type = GL_FRAGMENT_SHADER };
	GLuint nanosuit_program = Shaders_CreateShaderProgram(&nanosuit_pair[0], 2);

	/*
		Single, simple function call to load model into struct Model variable called nanosuit. See Types.h for struct Model decleration.
	*/
	struct Model *nanosuit = Model_Load("Objects/Nanosuit/nanosuit.obj");
	Camera *camera = Camera_Init();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	SDL_Event event;
	while (1)
	{
		if (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				break;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(nanosuit_program);
		{
			Camera_Render(camera, nanosuit_program);
			Model_Render(nanosuit, nanosuit_program);
		}
		glUseProgram(0);

		SDL_GL_SwapWindow(gl_window);
	}
	
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(gl_window);

	return 0;
}