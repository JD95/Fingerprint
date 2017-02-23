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

/*!
Generates a polygon based on the given verties.
Comes with a shader supporting a texture.
*/
class FullSCreen
{
	VertexArrayObject<Basic2dCoords> model;
	Shader<
		UniformSampler2D, 
		UniformVec2, 
		UniformFloat, 
		UniformInt
	> shader;

	std::vector<VertexData2D> vertices;
	Texture texture;

public:

	FullSCreen()
		: vertices { 
			{ { 0, 1, 0, 255 },{ -1.0f, 1.00f } },
			{ { 1, 0, 0, 255 },{ 1.00f, -1.00f } },
			{ { 0, 0, 0, 255 },{ -1.00f, -1.00f } },
			{ { 0, 1, 255, 255 },{ -1.00f, 1.00f } },
			{ { 1, 0, 255, 255 },{ 1.00f, -1.00f } },
			{ { 1, 1, 255, 255 },{ 1.00f, 1.00f } } }
		, texture("pal.png")
		, shader({ 
			"labrat/shader/programs/mandelbrot.vert", 
			"labrat/shader/programs/mandelbrot.frag", 
			NULL, NULL },
			[](GLuint program) { return std::make_tuple(
				UniformSampler2D(program, "tex"),
				UniformVec2(program, "center"),
				UniformFloat(program,"scale"),
				UniformInt(program,"iter")
			); })
	{
		model.bind();
		model.use_vbo<0>().setBufferData(vertices);
	}

	FullSCreen(const FullSCreen& other)
		: shader(other.shader)
	{
		std::cout << "COPY CONSTRUCTOR CALLED!!!!" << std::endl;
	}

	~FullSCreen() {};

	void render(const glm::vec2& center, const float& scale, const int& iter) {
		model.bind();
		shader.render_object(0, 1, GL_TRIANGLES, vertices.size(), std::make_tuple(0, texture.id()), center, scale, iter);
	}
};

