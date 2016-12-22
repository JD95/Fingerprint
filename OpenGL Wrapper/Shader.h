#pragma once

#include <string>
#include <GL\glew.h>

#include "VertexArrayObject.h"
#include "LoadShader.h"

class Shader
{
	static constexpr void* buffer_offset(int offset) {
		return (void *)(offset);
	}

	GLuint id;
	ShaderInfo info;
public:
	Shader(const char *vert, const char *frag, const char *tess, const char *geo)
		: info(vert, frag,tess,geo)
	{
		id = LoadShaders(info);
	};

	~Shader() {};

	void render_object(const int index, size_t object_size) const {
		glUseProgram(id);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, buffer_offset(0));
		glEnableVertexAttribArray(index);
		glDrawArrays(GL_TRIANGLES, 0, object_size);
	};
};

