#pragma once

#include <vector>
#include <array>
#include <string>
#include <iostream>

#include "../shader/shader.h"
#include "../shader/uniform.h"
#include "../opengl/vertex_array_object.h"
#include "../opengl/vertex_buffer_object.h"
#include "../texture/texture.h"
#include "../opengl/vertexdata.h"
#include "../reactive/reactive.h"

/*!
	Generates a polygon based on the given verties.
	Comes with a shader supporting a texture.
*/
class Polygon
{
	VertexArrayObject<Basic2dCoords> model;
	Shader<
		UniformMat4,		// MVP matrix
		UniformSampler2D,	// Texture
		UniformVec2,		// Sprite Box
		UniformVec2			// Sprite Position
	> shader;

	std::vector<VertexData2D> vertices;
	Texture texture;

public:

	/*!
		Defines the height and width of the sprite
		box within the texture.
	*/
	glm::vec2 sprite_box;

	/*!
		Defines the position of the sprite box from its
		top left corner with 
	*/
	Reactive<glm::vec2> sprite_position;

	Polygon(std::string texture_name, std::vector<VertexData2D> vs)
		: vertices(vs)
		, texture(texture_name)
		, sprite_box(1.0f,1.0f)
		, sprite_position(glm::vec2(0.0f,0.0f))
		, shader({ "labrat/shader/programs/basic.vert", "labrat/shader/programs/basic.frag", NULL, NULL }, 
			[](GLuint program) { return std::make_tuple(
				UniformMat4(program, "modelMatrix"),
				UniformSampler2D(program, "tex"),
				UniformVec2(program, "sprite_box"),
				UniformVec2(program, "sprite_position")
			);})
	{
		model.bind();
		model.use_vbo<0>().setBufferData(vertices);
	}

	Polygon(const Polygon& other) 
		: shader(other.shader)
	{
		std::cout << "COPY CONSTRUCTOR CALLED!!!!" << std::endl;
	}

	~Polygon() {};

	void render(const glm::mat4& mvp) {
		model.bind();
		shader.render_object(0, 1, GL_TRIANGLES, vertices.size(), mvp, std::make_tuple(0, texture.id()), sprite_box, sprite_position.value);
	}
};

