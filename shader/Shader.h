#pragma once

#include <string>
#include <tuple>

#include <GL\glew.h>

#include "../opengl/vertex_array_object.h"
#include "../shader/loadshader.h"
#include "../shader/uniform.h"
#include "../opengl/vertexdata.h"


struct ShaderLocations {
	char *vert; 
	char *frag; 
	char *tess; 
	char *geo;
};

constexpr void* buffer_offset(int offset) {
	return (void *)(offset);
}

// A simple shader that uses no uniform variables
class BasicShader
{
protected:

	GLuint program;
	ShaderInfo info;

	// TODO: Generalize this process, allow for different VBOs to be used
	void activate_shader(const GLuint vecLoc, const GLuint colorLoc) const {
		// Binds the current shader
		glUseProgram(program);

		/*  The particular buffer which will be associated with these values
			must be bound at the time of calling the below functions.

			If different buffers will be routed to the shader, then those buffers
			need to be bound before their attributes are enabled.

			To make things easier, we will be using a single struct to pass all
			of the relevant information and just tell OpenGL how to interperate
			the struct for what it needs.

		*/

		// Sets up pipe for vertex data to be sent to the shader
		glVertexAttribPointer(vecLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData2D), buffer_offset(sizeof(RGBA)));
		glEnableVertexAttribArray(vecLoc);

		// Sets up pipe for color data to be sent to the shader
		glVertexAttribPointer(colorLoc, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VertexData2D), buffer_offset(0));
		glEnableVertexAttribArray(colorLoc);
	}

	// TODO: Allow for draw settings to be customized
	void draw_object(const int draw_style, const size_t& object_size) const {
		glDrawArrays(draw_style, 0, object_size);
	}

public:

	BasicShader(ShaderLocations&& sl)
		: info(sl.vert, sl.frag, sl.tess, sl.geo)
	{
		program = LoadShaders(info);
	}

	~BasicShader() {};

	void render_object(const GLuint vecLoc, const GLuint colorLoc, const int draw_style, const size_t& object_size) const {
		activate_shader(vecLoc, colorLoc);
		draw_object(draw_style, object_size);
	}
};


// A normal shader that uses uniform variables
template <class ...Ts>
class Shader : BasicShader
{
	std::tuple<Ts...> uniform_variables;

public:

	template<class UniformsInit>
	Shader(ShaderLocations&& sl, UniformsInit&& f)
		: BasicShader(std::forward<ShaderLocations>(sl))
		, uniform_variables(f(program))
	{
	};

	~Shader() {};

	template <class ...Vs>
	void render_object(const GLuint& vecLoc, const GLuint& colorLoc, const int draw_style, const size_t& object_size, Vs&&... uniform_values) const {
		activate_shader(vecLoc, colorLoc);
		Uniform::set_values(uniform_variables, std::make_tuple(std::forward<Vs>(uniform_values)...));
		draw_object(draw_style, object_size);
	};
};


