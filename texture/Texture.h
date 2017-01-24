#pragma once

#include "GL\glew.h"
#include <string>

class Texture
{
	GLuint texture_id;
public:
	Texture(std::string texture_name);
	~Texture();

	GLuint id() const { return texture_id; }
};

