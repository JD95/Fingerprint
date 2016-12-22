#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>

#include "Checks.h"
#include "Type_Utitilies.h"

using std::vector;
using std::array;

template <int N>
using Coords = std::array<GLfloat, N>;

enum BufferType { 
	Array = GL_ARRAY_BUFFER
};

enum BufferUsage {
	Static_Draw = GL_STATIC_DRAW
};

/*
	Binds an ArrayBuffer and passes array values to GPU
*/
template <BufferType t, int col_size, BufferUsage u>
class BufferObject
{
	GLuint id;
public:
	BufferObject(const GLuint id)
	{
		this->id = id;
		glBindBuffer(t, id);
	}

	BufferObject(const GLuint id, const vector<Coords<col_size>> &data)
	{
		this->id = id;
		glBindBuffer(t, id);
		setBufferData(data);
	}

	~BufferObject() {}

	void setBufferData(const vector<Coords<col_size>> &data)
	{
		glBufferData(t, data.size() * sizeof(GLfloat) * col_size, &data[0], u);
	}
};

using Basic2dCoords = BufferObject<Array, 2, Static_Draw>;