#pragma once

#include <string>
#include <GL\glew.h>

#include "VertexArrayObject.h"
#include "LoadShader.h"
#include "Uniform.h"
#include <tuple>


struct ShaderLocations {
	char *vert; 
	char *frag; 
	char *tess; 
	char *geo;
};

// A simple shader that uses no uniform variables
class BasicShader
{
protected:
	static constexpr void* buffer_offset(int offset) {
		return (void *)(offset);
	}

	GLuint id;
	ShaderInfo info;

	void activate_shader(const int index) const {
		glUseProgram(id);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, buffer_offset(0));
		glEnableVertexAttribArray(index);
	}

	void draw_object(size_t object_size) const {
		glDrawArrays(GL_TRIANGLES, 0, object_size);
	}

public:

	BasicShader(ShaderLocations sl)
		: info(sl.vert, sl.frag, sl.tess, sl.geo)
	{
		id = LoadShaders(info);
	};

	~BasicShader() {};

	void render_object(const int index, size_t object_size) const {
		activate_shader(index);
		draw_object(object_size);
	};
};


// A normal shader that uses uniform variables
template <class ...Ts>
class Shader : BasicShader
{
	std::tuple<Ts...> uniform_variables;

public:

	template<class UniformsInit>
	Shader(ShaderLocations sl, UniformsInit f)
		: BasicShader(sl)
	{
		uniform_variables = f(id);
	};

	~Shader() {};

	template <class ...Vs>
	void render_object(const int index, size_t object_size, Vs&... values) const {
		activate_shader(index);
		Uniform::set_values(uniform_variables, values...);
		draw_object(object_size);
	};
};


