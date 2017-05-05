// C++ Headers
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <memory>
#include <chrono>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include "../opengl/vertex_array_object.h"
#include "../opengl/vertex_buffer_object.h"
#include "../shader/shader.h"
#include "../shader/loadshader.h"
#include "../scene/scene.h"
#include "../scene/camera.h"
#include "../primitive_shapes/full_screen.h"

using std::vector;
using std::array;



template<class T>
class Game {

	std::string programName;

	int window_width;
	int window_height;

	// Our SDL_Window ( just like with SDL2 wihout OpenGL)
	SDL_Window *mainWindow;

	// Our opengl context handle
	SDL_GLContext mainContext;

	#define BUFFER_OFFSET(offset) ((void *)(offset))

	bool init;

	bool Game::SetOpenGLAttributes()
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

	void Game::CheckSDLError(int line = -1)
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

	void Game::PrintSDL_GL_Attributes()
	{
		int value = 0;
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
		std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
		std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
	}

public:



	Game::Game(std::string name, int width, int height)	
		: window_width(width), window_height(height), programName(name)
	{
		init = false;

		// Initialize SDL's Video subsystem
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		{
			std::cout << "Failed to init SDL\n";
			return;
		}

		atexit(SDL_Quit);

		// Create our window centered at 512x512 resolution
		mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			window_width, window_height, SDL_WINDOW_OPENGL);

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

		// AUDIO SETUP

		// Set up the global audio stream
		int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
		if (result < 0)
		{
			fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
			exit(-1);
		}

		// Allocate set number of audio channels
		result = Mix_AllocateChannels(4);

		if (result < 0)
		{
			fprintf(stderr, "Unable to allocate mixing channels: %s\n", SDL_GetError());
			exit(-1);
		}

		init = true;
	}

	Game::~Game()
	{
		// Delete our OpengL context
		SDL_GL_DeleteContext(mainContext);

		// Destroy our window
		SDL_DestroyWindow(mainWindow);

		// Close audio channels
		Mix_CloseAudio();

		// Shutdown SDL 2
		SDL_Quit();
	}

	int game()
	{
		if (!init) return -1;

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// Because this isn't enabled by default...you fuckers
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_ONE);

		SDL_GL_SwapWindow(mainWindow);
		
		bool loop = true;

		Scene<T> scene;
		std::chrono::time_point<std::chrono::system_clock> start, end;
		std::chrono::nanoseconds delta_time;

		while (loop) {
			delta_time = end - start;
			start = std::chrono::system_clock::now();
			std::vector<SDL_Event> events;

			// Handle SDL events
			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					loop = false;

				if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP || event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
				{
					events.push_back(event);
				}	
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			scene.update(events, delta_time);

			if (scene.st.current_level.value == -1)
				loop = false;

			glFlush();

			SDL_GL_SwapWindow(mainWindow);
			end = std::chrono::system_clock::now();
		}

		return 0;
	}
};