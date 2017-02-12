#pragma once

#include <string>
#include <vector>

#include "../opengl/VertexData.h"


struct Model
{
	std::string model_name;
	std::vector<VertexData2D> tex_coords;
public:
	Model();
	Model(std::string name, std::vector<VertexData2D> ts);
	~Model();
};

