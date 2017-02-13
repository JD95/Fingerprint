#pragma once

#include "GL\glew.h"
#include <string>

/*!
	Loads a PNG image from the given file path. 
	The image must have interlacing disabled to load properly.
*/
class Texture
{
	GLuint texture_id;
public:
	Texture();
	Texture(std::string file_path);
	~Texture();

	GLuint id() const { return texture_id; }
};

