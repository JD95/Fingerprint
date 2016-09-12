#pragma once

#include <GL\glew.h>

template <int n> class VertexArrayObject
{
	GLuint ids[n];

public:

	VertexArrayObject()
	{
		glGenVertexArrays(n, ids);
	}

	~VertexArrayObject()
	{
		glDeleteVertexArrays(n, ids);
	}

	void bindVertexArray(const int index)
	{
		//static_assert(index > 0 && index < n, "Vertex Array Index out of bounds");
		glBindVertexArray(index);
	}
};

