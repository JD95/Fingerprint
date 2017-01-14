#include "Window.h"
#include "Polygon.h"
#include "Transform.h"
#include "Camera.h"
#include "Units.h"
#include <ctime>

Window::Window()
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

inline void rotate_camera(Camera& c, float x, float y, float z) {
	auto r = glm::quat(glm::vec3(x, y, z));
	c.focus = (r * (c.focus - c.position)) + c.position; // calculate rotated point
	//c.up_direction = r * c.up_direction;
	//c.rotation = glm::rotate(c.rotation, r.w, glm::axis(r));
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
		rotate_camera(c,  camera_speed, 0, 0);
		break;
	case SDLK_DOWN:
		rotate_camera(c, -1 * camera_speed, 0, 0);
		break;
	case  SDLK_LEFT:
		rotate_camera(c, 0, camera_speed, 0);
		break;
	case SDLK_RIGHT:
		rotate_camera(c, 0, -1 * camera_speed, 0);
		break;
	default: break;
	}
}

void Window::RunGame()
{
	bool loop = true;

	Polygon shape({
		{{ 0, 255, 0, 255 },{ -0.90f, 0.90f }},
		{{ 0, 255, 0, 255 },{  0.90f, -0.90f }},
		{{ 0, 255, 0, 255 },{ -0.90f, -0.90f }},
		{{ 0, 0, 255, 255 },{ -0.90f, 0.90f }},
		{{ 0, 0, 255, 255 },{ 0.90f, -0.90f }},
		{{ 0, 0, 255, 255 },{ 0.90f, 0.90f }},
	});

	Transform t;

	t.position = glm::vec3(0, 0.0f, 0);
	t.scale = glm::vec3(1.0f, 1.0f, 1.0f);

	Camera c( glm::vec3(0, 0.0f, 1.0f)		// Position
			, glm::vec3(0.0, 0.5f, -10.0f)	// Focus
			, glm::vec3(0, 1.0f, 0)			// Up
			);

	for (float i = 0; loop; i += 0.01f)
	{
		// Draw triangles
		glClear(GL_COLOR_BUFFER_BIT);

		//auto delta = (float)glm::sin(i) * 0.005f;
		//t.position[0] += delta;
		//t.scale += delta;
		//t.rotation = glm::rotate(t.rotation, 0.01f, glm::vec3(0.0f, 0.0f, 1.0f));
		//c.position[0] += 0.01f;
		//c.center[0] += 0.01f;
		
		//const auto p = glm::frustum( -window_width / 2.0f, window_width / 2.0f		// View width
		//						   , -window_height / 2.0f, window_height / 2.0f	// View height
		//						   , 0.1f											// View near
		//						   , 10.0f);										// View far
		//const auto p = glm::infinitePerspective(to_radians(45), 5.0f, 0.05f);
		//const auto p = glm::perspective(to_radians(45), 1.0f, 0.05f, 10.0f);
		//const auto p = glm::ortho(-10.0f,10.0f,-10.0f,10.0f);

		shape.render(c.perspective_projection(45.0f,1.0f,0.5f) * c.view_matrix() * t.model_matrix());
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