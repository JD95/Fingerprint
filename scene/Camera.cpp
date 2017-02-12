#include "camera.h"



Camera::Camera()
{
}

Camera::Camera(glm::vec3 p, glm::vec3 c, glm::vec3 u)
	: position(p), focus(c), up_direction(u)
{
}


Camera::~Camera()
{
}

glm::mat4 Camera::view_matrix(){
	return glm::lookAt(position, focus, up_direction);
}

// TODO: Fix bugs
glm::mat4 Camera::ortho_projection(float left, float right, float bottom, float top){
	auto x = position[0];
	auto y = position[1];
	return glm::ortho(x - left, x + right, y - bottom, y + top, 1.0f,10.0f);
}

glm::mat4 Camera::perspective_projection(float radians, float aspect, float near) {
	return glm::perspective(radians, 1.0f, 0.05f, glm::distance(position, focus));
}


void Camera::rotate_camera(float x, float y, float z) {
	auto r = glm::quat(glm::vec3(x, y, z));
	focus = (r * (focus - position)) + position; // calculate rotated point
}