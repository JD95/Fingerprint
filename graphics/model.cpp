#include "model.h"



Model::Model()
{
}

Model::Model(std::string name, std::vector<VertexData2D> ts)
	: model_name(name), tex_coords(ts) {}

Model::~Model()
{
}
