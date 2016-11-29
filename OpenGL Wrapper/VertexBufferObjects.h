#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>

#include "Checks.h"
#include "Type_Utitilies.h"

using std::vector;
using std::array;

/*
	Manages the lifetime of many buffer objects
*/
template <class ...T>
class BufferObjectCollection
{
private:
	static constexpr int slot_size = sizeof...(T);
	GLuint idArray[slot_size];

public:
	BufferObjectCollection()
	{
		glGenBuffers(slot_size, idArray);
	}

	~BufferObjectCollection()
	{
		glDeleteBuffers(slot_size, idArray);
	}

	template <int index = 0, class = std::enable_if_t<within_range(index, 0, slot_size)>>
	GLuint get_id() const
	{
		return idArray[index];
	}

	template <int index, class T = type_index<index, T...>::type>
	auto get_vbo() -> T
	{
		return std::move(T(get_id<index>()));
	}
};

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

	BufferObject(const GLuint id, const vector<array<GLfloat, col_size>> &data)
	{
		this->id = id;
		glBindBuffer(t, id);
		setBufferData(data);
	}

	~BufferObject() {}

	void setBufferData(const vector<array<GLfloat, col_size>> &data)
	{
		glBufferData(t, data.size() * sizeof(GLfloat) * col_size, &data[0], u);
	}
};
