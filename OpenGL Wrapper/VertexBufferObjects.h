#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>

using std::vector;
using std::array;

/*
	Manages the lifetime of many buffer objects
*/
template <int n>
class VboManager
{
private:
	GLuint idArray[n];

public:
	VboManager()
	{
		glGenBuffers(n, idArray);
	}

	~VboManager()
	{
		glDeleteBuffers(n, idArray);
	}

	GLuint getID(const int index)
	{
		return idArray[index];
	}

};


/*
	Binds an ArrayBuffer and passes array values to GPU
*/
template <int n>
class ArrayBufferObject
{
	GLuint id;
public:
	ArrayBufferObject(const GLuint id) 
	{
		this->id = id;
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	ArrayBufferObject(const GLuint id, const vector<array<GLfloat, n>> &data)
	{
		this->id = id;
		glBindBuffer(GL_ARRAY_BUFFER, id);
		setArrayBufferData(data);
	}

	~ArrayBufferObject() {}

	void setArrayBufferData(const vector<array<GLfloat,n>> &data)
	{
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat) * n, &data[0], GL_STATIC_DRAW);
	}
};
