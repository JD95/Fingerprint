#pragma once

#include <glm.hpp>
#include <gtc\quaternion.hpp>

#include "../reactive/reactive.h"

struct Transform
{
	Reactive<glm::vec3> position;
	Reactive<glm::vec3> scale;
	Reactive<glm::quat> rotation;


	Transform();
	Transform(glm::vec3 p);
	Transform(glm::vec3 p, glm::vec3 s);
	Transform(glm::vec3 p, glm::vec3 s, glm::quat r);
	~Transform();

	glm::mat4 model_matrix();
};

