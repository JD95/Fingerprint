#include "model.h"



Model::Model()
{
}

Model::Model(std::string name, std::vector<VertexData2D> ts)
	: model_name(name), tex_coords(ts) {}

Model::~Model()
{
}

AnimatedModel::AnimatedModel()
{
}

AnimatedModel::AnimatedModel(std::string name, unsigned int r, unsigned int c, std::vector<Animation> as, std::vector<VertexData2D> vs)
	: Model(name, vs)
	, rows(r)
	, cols(c)
	, animations(as)
{
}
