#include "transform.h"

#include <gtc\matrix_transform.hpp>


Transform::Transform()
	: scale(1.0f, 1.0f,1.0f)
{
}

Transform::Transform(glm::vec3 p)
	: position(p)
{
}

Transform::Transform(glm::vec3 p, glm::vec3 s)
	: position(p), scale(s)
{
}

Transform::Transform(glm::vec3 p, glm::vec3 s, glm::quat r)
	: position(p), scale(s), rotation(r)
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
