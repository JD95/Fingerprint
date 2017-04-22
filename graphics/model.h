#pragma once

#include <string>
#include <vector>

#include "../opengl/VertexData.h"
#include "../texture/SpriteSheet.h"


struct Model
{
	std::string model_name;
	std::vector<VertexData2D> tex_coords;
public:
	Model();
	Model(std::string name, std::vector<VertexData2D> ts);
	~Model();
};


struct AnimatedModel : Model {

	unsigned int rows;
	unsigned int cols;
	std::vector<Animation> animations;

	AnimatedModel();
	AnimatedModel( 
		std::string name, 
		unsigned int rows,
		unsigned int cols, 
		std::vector<Animation> animations, 
		std::vector<VertexData2D> vs);
};
