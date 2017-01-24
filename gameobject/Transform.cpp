#include "Transform.h"

#include <glm\gtc\matrix_transform.hpp>


Transform::Transform()
	: scale(1.0f, 1.0f,1.0f)
{
}


Transform::~Transform()
{
}



glm::mat4 Transform::model_matrix()
{
	glm::mat4 model_m;

	model_m = glm::scale(model_m, scale);
	model_m *= glm::mat4_cast(rotation);
	model_m = glm::translate(model_m, position);
	return model_m;
}
