// C++ Headers
#include <string>
#include <iostream>
#include <SDL.h>

#include "Window.h"

#include <tuple>
#include "Uniform.h"

int main(int argc, char *argv[])
{
	auto uniforms = std::make_tuple(UniformInt((GLint)1), UniformInt((GLint)2));
	auto values = std::make_tuple((GLint)5, (GLint)5);
	Uniform::set_values(uniforms, values);

	//physics_tests();
	//return game();
	//system("pause4");
	Window window;
	window.game();
	return 0;
}