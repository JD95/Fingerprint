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

	GLuint program;
	ShaderInfo info;

	void activate_shader() const {
		// Binds the current shader
		glUseProgram(program);

		// Sets up pipe for vertex data to be sent to the shader
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, buffer_offset(0));
		glEnableVertexAttribArray(0);

		// Sets up pipe for color data to be sent to the shader
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, buffer_offset(0));
		glEnableVertexAttribArray(1);
	}

	void draw_object(const size_t& object_size) const {
		glDrawArrays(GL_TRIANGLES, 0, object_size);
	}

public:

	BasicShader(ShaderLocations&& sl)
		: info(sl.vert, sl.frag, sl.tess, sl.geo)
	{
		program = LoadShaders(info);
	};

	~BasicShader() {};

	void render_object(const size_t& object_size) const {
		activate_shader();
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
	Shader(ShaderLocations&& sl, UniformsInit f)
		: BasicShader(sl)
	{
		uniform_variables = f(program);
	};

	~Shader() {};

	template <class ...Vs>
	void render_object(const size_t& object_size, Vs&&... values) const {
		activate_shader();
		Uniform::set_values(uniform_variables, std::forward(values)...);
		draw_object(object_size);
	};
};


