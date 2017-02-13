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
class Polygon
{
	VertexArrayObject<Basic2dCoords> model;
	Shader<UniformMat4, UniformSampler2D> shader;

	std::vector<VertexData2D> vertices;
	Texture texture;

public:

	Polygon(std::string texture_name, std::vector<VertexData2D> vs)
		: vertices(vs)
		, texture(texture_name)
		, shader({ "labrat/shader/programs/basic.vert", "labrat/shader/programs/basic.frag", NULL, NULL }, 
			[](GLuint program) { return std::make_tuple(
				UniformMat4(program, "modelMatrix"),
				UniformSampler2D(program, "tex")
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
		shader.render_object(0, 1, GL_TRIANGLES, vertices.size(), mvp, std::make_tuple(0, texture.id()));
	}
};

