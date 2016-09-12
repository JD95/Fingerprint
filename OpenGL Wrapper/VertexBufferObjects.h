#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>

using std::vector;
using std::array;

class VertexBufferObject
{
private:
	GLuint idArray[1];

protected:
	GLuint id;

public:
	VertexBufferObject()
	{
		glGenBuffers(1, idArray);
		id = idArray[0];
	}

	~VertexBufferObject()
	{
		glDeleteBuffers(1, idArray);
	}
};


template <int n>
class ArrayBufferObject : public VertexBufferObject
{
public:
	ArrayBufferObject() {}
	~ArrayBufferObject() {}

	void bindArrayBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void setArrayBufferData(const vector<array<GLfloat,n>> &data)
	{
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat) * n, &data[0], GL_STATIC_DRAW);
	}
};
