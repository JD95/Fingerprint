#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;


	Transform();
	~Transform();

	glm::mat4 model_matrix();
};

