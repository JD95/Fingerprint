// C++ Headers
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <fstream>

#include <GL/glew.h>
#include <SDL.h>

#include "../opengl/vertex_array_object.h"
#include "../opengl/vertex_buffer_object.h"
#include "../shader/shader.h"
#include "../shader/loadshader.h"


using std::vector;
using std::array;



class Window {

	std::string programName;

	int window_width;
	int window_height;

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

	void RunGame();


public:
	Window(std::string name, int width, int height);

	~Window();

	int game();
};