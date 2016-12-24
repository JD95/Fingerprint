#pragma once

#include <vector>
#include <array>

#include "Shader.h"
#include "Uniform.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

class Polygon
{
	VertexArrayObject<Basic2dCoords> model;
	Shader<UniformUint> shader;

	std::vector<VertexData2D> vertices;

public:
	Polygon(std::vector<VertexData2D> vs)
		: vertices(vs)
		, shader({ "triangles.vert", "triangles.frag", NULL, NULL }, 
			[](GLuint program) { return std::make_tuple(
				UniformUint(glGetUniformLocation(program, "test"))
			);})
	{
		model.bind();
		model.use_vbo<0>().setBufferData(vertices);
	}

	~Polygon();

	void render() {
		model.bind();
		shader.render_object(0, 1, GL_TRIANGLES, vertices.size(), 255);
	}
};

