#pragma once

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"

//#include "Fingerprint/gameobject/Transform.h"

/*
	A camera is responsible for generating a projection matrix.

	TODO:
	- Add orthographic projection options
	- Add utility functions for changing camera attributes
*/

struct Camera
{
	glm::vec3 position;
	glm::vec3 focus;
	glm::vec3 up_direction;

public:
	Camera();
	Camera(glm::vec3 p, glm::vec3 c, glm::vec3 u);
	~Camera();

	glm::mat4 view_matrix();
	glm::mat4 ortho_projection(float left, float right, float bottom, float top);
	glm::mat4 perspective_projection(float radians, float aspect, float near);

};

