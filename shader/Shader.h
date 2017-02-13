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

/*! 
	A simple shader that uses no uniform variables.
*/
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

	BasicShader(ShaderInfo&& sl)
		: info(sl)
	{
		program = LoadShaders(info);
	}

	BasicShader(ShaderInfo locations, GLuint shader_id)
		:info(locations), program(shader_id) {}

	BasicShader(const BasicShader& other) {
		this->info = other.info;
		this->program = other.program;
	}

	~BasicShader() {};

	/*!
		Renders the vertices in the currenlty bound VBO using the provided shaders.
	*/
	void render_object(const GLuint vecLoc, const GLuint colorLoc, const int draw_style, const size_t& object_size) const {
		activate_shader(vecLoc, colorLoc);
		draw_object(draw_style, object_size);
	}
};


/*! 
	A normal shader that uses uniform variables.
*/
template <class ...Ts>
class Shader : BasicShader
{
	std::tuple<Ts...> uniform_variables;

public:

	/*!
		Constructs a shader program using the given glsl files
		and a function taking a GLuint (the program id) and a tuple
		of all the uniform values to be used with the shader.

		@code
		// Model constructor
		ExampleModel(std::string texture_name, std::vector<VertexData2D> vs)
			: vertices(vs)
			, texture(texture_name)
			, shader({ "basic.vert", "basic.frag", NULL, NULL },
				[](GLuint program) { return std::make_tuple(
					UniformMat4(program, "modelMatrix"),
					UniformSampler2D(program, "tex")
			);}) { ... }

		// basic.vert
		#version 430 core

		layout(location = 0) in vec4 vPosition;
		layout(location = 1) in vec4 vColor;

		uniform mat4 modelMatrix;

		out vec4 color;

		void main() {
			color = vColor;
			gl_Position = modelMatrix * vPosition;
		}

		// basic.frag
		#version 430 core

		uniform sampler2D tex;

		in vec4 color;

		out vec4 fColor;

		void main() {
			fColor = texture(tex, vec2(color[0],color[1]));
		}

		@endcode

	*/
	template<class UniformsInit>
	Shader(ShaderInfo&& sl, UniformsInit&& f)
		: BasicShader(std::forward<ShaderInfo>(sl))
		, uniform_variables(f(program))
	{
	};
	

	Shader(const Shader& other) 
		: BasicShader(other.info, other.program){
		this->uniform_variables = other.uniform_variables;
	}

	~Shader() {}

	/*!
		Renders the vertices in the currenlty bound VBO using the provided shaders and the
		given uniform variables.

		@code

		class ExampleModel
		{
			VertexArrayObject<Basic2dCoords> model;
			Shader<UniformMat4, UniformSampler2D> shader;

			std::vector<VertexData2D> vertices;
			Texture texture;

			...

			void render(const glm::mat4& mvp) {
				model.bind();
				shader.render_object(
					0, 
					1, 
					GL_TRIANGLES, 
					vertices.size(),
					mvp,								// Uniform Value 1
					std::make_tuple(0, texture.id())	// Uniform Value 2
				);
			}
		};

		@endcode
	*/
	template <class ...Vs>
	void render_object(const GLuint& vecLoc, const GLuint& colorLoc, const int draw_style, const size_t& object_size, Vs&&... uniform_values) const {
		activate_shader(vecLoc, colorLoc);
		Uniform::set_values(uniform_variables, std::make_tuple(std::forward<Vs>(uniform_values)...));
		draw_object(draw_style, object_size);
	}
};


