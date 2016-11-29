#include "Window.h"

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

template <int n>
void Window::display(VertexArrayObject<n> &vao, vector<array<GLfloat, 2>> verts) {
	glClear(GL_COLOR_BUFFER_BIT);
	vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, verts.size());
	glFlush();
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

void Window::RunGame()
{
	bool loop = true;

	VertexArrayObject<1> vao;
	vao.bind<0>();

	vector<array<GLfloat, 2>> vertices =
	{ { -0.90f, -0.90f } // Triangle 1 
		,{ 0.85f, -0.90f }
		,{ -0.90f, 0.85f }
		,{ 0.90f, -0.85f } // Triangle 2 
		,{ 0.90f, 0.90f }
		,{ -0.85f, 0.90f }
	};

	BufferObjectCollection<
		BufferObject<BufferType::Array, 2,
		BufferUsage::Static_Draw>
	> vbo;

	auto verts = vbo.get_vbo<0>();
	verts.setBufferData(vertices);

	ShaderInfo info("triangles.vert", "triangles.frag", NULL, NULL);
	GLuint program = LoadShaders(info);
	glUseProgram(program);

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	while (loop)
	{
		display<1>(vao, vertices);
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
				default: break;
				}
			}
		}
		SDL_GL_SwapWindow(mainWindow);
	}
}

int Window::game()
{
	if (!init) return -1;

	// Clear our buffer with a black background
	// This is the same as :
	// 		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
	// 		SDL_RenderClear(&renderer);
	//
	glClearColor(0.0, 0.75, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mainWindow);

	RunGame();

	return 0;
}