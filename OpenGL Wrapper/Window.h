// C++ Headers
#include <string>
#include <iostream>
#include <vector>
#include <array>

#include <GL/glew.h>
#include <SDL.h>

#include "VertexArrayObject.h"
#include "VertexBufferObjects.h"

#include "LoadShader.h"

#include "PhysicsTesting.h"
#include <fstream>

using std::vector;
using std::array;

class Window {

	std::string programName = "Headerphile SDL2 - OpenGL thing";

	// Our SDL_Window ( just like with SDL2 wihout OpenGL)
	SDL_Window *mainWindow;

	// Our opengl context handle
	SDL_GLContext mainContext;

	enum Attrib_IDs { vPosition = 0 };

	#define BUFFER_OFFSET(offset) ((void *)(offset))

	bool init;

	bool SetOpenGLAttributes();

	void CheckSDLError(int line = -1);

	void PrintSDL_GL_Attributes();

	template <int n>
	void display(VertexArrayObject<n> &vao, vector<array<GLfloat, 2>> verts);

	void RunGame();

public:
	Window()
	{
		init = false;

		// Initialize SDL's Video subsystem
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "Failed to init SDL\n";		
			return;
		}

		// Create our window centered at 512x512 resolution
		mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			512, 512, SDL_WINDOW_OPENGL);

		// Check that everything worked out okay
		if (!mainWindow)
		{
			std::cout << "Unable to create window\n";
			CheckSDLError(__LINE__);
			return;
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

		init = true;
	}

	~Window()
	{
		// Delete our OpengL context
		SDL_GL_DeleteContext(mainContext);

		// Destroy our window
		SDL_DestroyWindow(mainWindow);

		// Shutdown SDL 2
		SDL_Quit();
	}

	int game();
};