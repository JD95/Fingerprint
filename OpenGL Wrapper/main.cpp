// C++ Headers
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <SDL.h>

#include "LoadShader.h"
std::string programName = "Headerphile SDL2 - OpenGL thing";

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;

// Our opengl context handle
SDL_GLContext mainContext;


enum VAO_IDs { Triangles, NumVAOs }; 
enum Buffer_IDs { ArrayBuffer, NumBuffers }; 
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs]; 
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

#define BUFFER_OFFSET(offset) ((void *)(offset))

bool SetOpenGLAttributes();
void PrintSDL_GL_Attributes();
void CheckSDLError(int line);
void RunGame();
void Cleanup();


bool SDL_Init()
{
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	// Create our window centered at 512x512 resolution
	mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		512, 512, SDL_WINDOW_OPENGL);

	// Check that everything worked out okay
	if (!mainWindow)
	{
		std::cout << "Unable to create window\n";
		CheckSDLError(__LINE__);
		return false;
	}

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow);

	SetOpenGLAttributes();

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	// Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	return true;
}

bool SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 4.3 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

int main(int argc, char *argv[])
{
	if (!SDL_Init())
		return -1;

	// Clear our buffer with a black background
	// This is the same as :
	// 		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
	// 		SDL_RenderClear(&renderer);
	//
	glClearColor(0.0, 0.75, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mainWindow);

	RunGame();

	Cleanup();

	return 0;
}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAOs[Triangles]); 
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
}


void RunGame()
{
	bool loop = true;

	glGenVertexArrays(NumVAOs, VAOs); 

	glBindVertexArray(VAOs[Triangles]);

	GLfloat vertices[NumVertices][2] = { { -0.90, -0.90 } // Triangle 1 
									   , { 0.85, -0.90 } 
									   , { -0.90, 0.85 }
									   , { 0.90, -0.85 } // Triangle 2 
									   , { 0.90, 0.90  } 
									   , { -0.85, 0.90 } 
									   };

	glGenBuffers(NumBuffers, Buffers); 

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	ShaderInfo info("triangles.vert", "triangles.frag");
	GLuint program = LoadShaders(info);
	glUseProgram(program);

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); 
	glEnableVertexAttribArray(vPosition);

	while (loop)
	{
		display();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				default:
					break;
				}
			}
		}
		SDL_GL_SwapWindow(mainWindow);
		// Swap our back buffer to the front
		// This is the same as :
		// 		SDL_RenderPresent(&renderer);
	}
}

void Cleanup()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy our window
	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit();
}

void CheckSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
	{
		std::cout << "SLD Error : " << error << std::endl;

		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;

		SDL_ClearError();
	}
}

void PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}