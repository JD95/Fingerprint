#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>

#include "../Utilities/Checks.h"
#include "../Utilities/Type_Utitilies.h"
#include "../opengl/VertexData.h"

using std::vector;
using std::array;

enum BufferType { 
	Array = GL_ARRAY_BUFFER
};

enum BufferUsage {
	Static_Draw = GL_STATIC_DRAW
};

/*
	Binds an ArrayBuffer and passes array values to GPU
*/
template <BufferType t, BufferUsage u>
class BufferObject
{
	GLuint id;
public:
	BufferObject(const GLuint id)
	{
		this->id = id;
		glBindBuffer(t, id);
	}

	BufferObject(const GLuint id, const vector<VertexData2D> &data)
	{
		this->id = id;
		glBindBuffer(t, id);
		setBufferData(data);
	}

	~BufferObject() {}

	void setBufferData(const vector<VertexData2D> &data)
	{
		glBufferData(t, data.size() * sizeof(VertexData2D), &data[0], u);
	}

	GLuint get_id() const { return id; }
};


using RgbaCoords = BufferObject<Array, Static_Draw>;
using Basic2dCoords = BufferObject<Array, Static_Draw>;
using BasicArray = BufferObject<Array, Static_Draw>;