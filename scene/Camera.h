#pragma once

#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\quaternion.hpp"

/*!
	A camera is responsible for generating a projection matrix.

*/
struct Camera
{
	glm::vec3 position;		/**< The position of the camera in the game world.*/
	glm::vec3 focus;		/**< The point the camera is looking at.*/
	glm::vec3 up_direction; /**< A vector indicating which direction is up. Typically (0,1,0). */

public:
	Camera();
	Camera(glm::vec3 p, glm::vec3 c, glm::vec3 u);
	~Camera();

	/*!
		Generates a view matrix based on the position, focus, and up direction.

		explanation of view matrices: http://www.3dgep.com/understanding-the-view-matrix/
	*/
	glm::mat4 view_matrix();
	glm::mat4 ortho_projection(float left, float right, float bottom, float top);

	/*!
		Generates a perspective matrix.

		explanation of projection matrices: https://en.wikipedia.org/wiki/3D_projection#Perspective_projection
	*/
	glm::mat4 perspective_projection(float radians, float aspect, float near);

	/*!
		Rotates the camera x, y, and z degrees around their respective angles.
	*/
	void rotate_camera(float x, float y, float z);
};

