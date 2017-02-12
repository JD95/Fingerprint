#include "window.h"

#include <ctime>

#include "../primitive_shapes/polygon.h"
#include "../entity/transform.h"
#include "../scene/camera.h"
#include "../utilities/units.h"
#include "../entity/entity.h"
#include "../graphics/model.h"
#include "../opengl/vertexdata.h"

Window::Window(std::string name, int width, int height)
	: window_width(width), window_height(height), programName(name)
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

	init = true;
}

Window::~Window()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy our window
	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit();
}

bool Window::SetOpenGLAttributes()
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

void Window::CheckSDLError(int line)
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

void Window::PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}


void move_camera_bindings(Camera& c, SDL_Event event) {

	// Negative because we are moving the world not the camera
	const float camera_speed = 0.1f;
	switch (event.key.keysym.sym)
	{
	case SDLK_w:
		c.position[1] += camera_speed;
		c.focus[1] += camera_speed;
		break;
	case SDLK_s:
		c.position[1] -= camera_speed;
		c.focus[1] -= camera_speed;
		break;
	case  SDLK_a:
		c.position[0] -= camera_speed;
		c.focus[0] -= camera_speed;
		break;
	case SDLK_d:
		c.position[0] += camera_speed;
		c.focus[0] += camera_speed;
		break;
	case SDLK_UP:
		c.rotate_camera(camera_speed, 0, 0);
		break;
	case SDLK_DOWN:
		c.rotate_camera(-1 * camera_speed, 0, 0);
		break;
	case  SDLK_LEFT:
		c.rotate_camera(0, camera_speed, 0);
		break;
	case SDLK_RIGHT:
		c.rotate_camera(0, -1 * camera_speed, 0);
		break;
	default: break;
	}
}

void Window::RunGame()
{
	bool loop = true;

	Entity picture(
		Transform(glm::vec3(0, 0.0f, 0),		// Position
				  glm::vec3(1.0f, 1.0f, 1.0f)), // Scale
		Model{ std::string("logo.png"),
			   std::vector<VertexData2D> { 
				{ { 0, 1, 0, 255 },{ -0.90f, 0.90f }},
				{ { 1, 0, 0, 255 },{ 0.90f, -0.90f } },
				{ { 0, 0, 0, 255 },{ -0.90f, -0.90f } },
				{ { 0, 1, 255, 255 },{ -0.90f, 0.90f } },
				{ { 1, 0, 255, 255 },{ 0.90f, -0.90f } },
				{ { 1, 1, 255, 255 },{ 0.90f, 0.90f } },
			}
		});

	Camera c( glm::vec3(0, 0.0f, 2.0f)		// Position
			, glm::vec3(0.0, 0.5f, -10.0f)	// Focus
			, glm::vec3(0, 1.0f, 0)			// Up
			);

	for (float i = 0; loop; i += 0.01f)
	{
		// Draw triangles
		glClear(GL_COLOR_BUFFER_BIT);

		auto mvp = c.perspective_projection(45.0f, 1.0f, 0.5f) 
				 * c.view_matrix() 
				 * picture.transform->model_matrix();

		picture.model->render(mvp);
		glFlush();

		// Handle SDL events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;

			if (event.type == SDL_KEYDOWN)
			{
				move_camera_bindings(c, event);
			}
		}

		SDL_GL_SwapWindow(mainWindow);
	}
}

int Window::game()
{
	if (!init) return -1;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mainWindow);

	RunGame();

	return 0;
}