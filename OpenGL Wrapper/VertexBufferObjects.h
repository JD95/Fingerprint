#pragma once

#include <GL/glew.h>

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

	void setArrayBufferData(GLfloat[][n] data, int usage)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, usage);
	}
};
