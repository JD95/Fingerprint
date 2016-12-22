// C++ Headers
#include <string>
#include <iostream>
#include <vector>
#include <array>

#include <GL/glew.h>
#include <SDL.h>

#include "VertexArrayObject.h"
#include "VertexBufferObjects.h"
#include "Shader.h"

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

	void display(const Shader& shader, size_t vector_size);

	void RunGame();

public:
	Window();

	~Window();

	int game();
};