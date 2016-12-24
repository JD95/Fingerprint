#pragma once

#include <GL\glew.h>
#include "Checks.h"

/*
	Manages the lifetime of a Vertex Array Object.
	These objects hold the geometry information for a
	given model in various vertex buffer objects.
*/
template <class ...T> class VertexArrayObject
{
	GLuint vao_id;
	static constexpr int num_vbos = sizeof...(T);
	GLuint vbo_ids[num_vbos];

public:

	VertexArrayObject()
	{
		glGenVertexArrays(1, &vao_id);
		glGenBuffers(num_vbos, vbo_ids);
	}

	~VertexArrayObject()
	{
		glDeleteVertexArrays(1, &vao_id);
	}

	void bind()
	{
		glBindVertexArray(vao_id);
	}

	template <int index, class T = type_index<index, T...>::type>
	auto use_vbo() -> T
	{
		return std::move(T(get_id<index>()));
	}

	template <int index = 0, class = std::enable_if_t<within_range(index, 0, num_vbos)>>
	GLuint get_id() const
	{
		return vbo_ids[index];
	}
};