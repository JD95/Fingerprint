#pragma once

#include <GL\glew.h>

template <int n> class VaoManager
{
	GLuint ids[n];

public:

	VaoManager()
	{
		glGenVertexArrays(n, ids);
	}

	~VaoManager()
	{
		glDeleteVertexArrays(n, ids);
	}

	GLuint getID(const int index)
	{
		return ids[index];
	}

};


class VertexArrayObject
{
	GLuint id;

public:
	VertexArrayObject(GLuint id)
	{
		this->id = id;
		glBindVertexArray(id);
	}
};
