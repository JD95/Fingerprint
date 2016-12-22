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
	BasicShader shader;

	std::vector<Coords<2>> vertices;

public:
	template <class ...Ts>
	Polygon(Ts&&... vs) 
		: vertices({ std::forward<Ts>(vs)... })
		, shader({ "triangles.vert", "triangles.frag", NULL, NULL })
	{
		model.bind();
		auto verts = model.use_vbo<0>();
		verts.setBufferData(vertices);
	}

	~Polygon();

	void render() {
		model.bind();
		shader.render_object(vertices.size());
	}
};

