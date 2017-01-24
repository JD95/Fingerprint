#pragma once

#include <vector>
#include <array>
#include <string>

#include "../shader/Shader.h"
#include "../shader/Uniform.h"
#include "../opengl/VertexArrayObject.h"
#include "../opengl/VertexBufferObject.h"
#include "../texture/Texture.h"
#include "../opengl/VertexData.h"

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
		, shader({ "Fingerprint/shader/programs/basic.vert", "Fingerprint/shader/programs/basic.frag", NULL, NULL }, 
			[](GLuint program) { return std::make_tuple(
				UniformMat4(program, "modelMatrix"),
				UniformSampler2D(program, "tex")
			);})
	{
		model.bind();
		model.use_vbo<0>().setBufferData(vertices);
	}

	~Polygon() {};

	// TODO: Pass in the MVP matrix
	void render(const glm::mat4& mvp) {
		model.bind();
		shader.render_object(0, 1, GL_TRIANGLES, vertices.size(), mvp, std::make_tuple(0, texture.id()));
	}
};

