#pragma once

#include <GL\glew.h>
#include "Checks.h"

/*
	Manages the lifetime of a Vertex Array Object.
	These objects hold the geometry information for a
	given model.
*/
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

	template <int index = 0, class = std::enable_if_t<within_range(index, 0, n)>>
	GLuint getID() const
	{
		return ids[index];
	}

	template <int index = 0, class = std::enable_if_t<within_range(index, 0, n)>>
	void bind()
	{
		glBindVertexArray(ids[index]);
	}
};