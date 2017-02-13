#include "window.h"

#include <ctime>

#include "../primitive_shapes/polygon.h"
#include "../entity/transform.h"
#include "../scene/camera.h"
#include "../utilities/units.h"
#include "../entity/entity.h"
#include "../graphics/model.h"
#include "../opengl/vertexdata.h"


void move_camera_bindings(Camera& c, SDL_Event event) {

	// Negative because we are moving the world not the camera
	const float camera_speed = 0.1f;
	switch (event.key.keysym.sym)
	{
	case SDLK_w:
		c.position[1] += camera_speed;
		c.focus[1] += camera_speed;
		break;
	case SDLK_s:
		c.position[1] -= camera_speed;
		c.focus[1] -= camera_speed;
		break;
	case  SDLK_a:
		c.position[0] -= camera_speed;
		c.focus[0] -= camera_speed;
		break;
	case SDLK_d:
		c.position[0] += camera_speed;
		c.focus[0] += camera_speed;
		break;
	case SDLK_UP:
		c.rotate_camera(camera_speed, 0, 0);
		break;
	case SDLK_DOWN:
		c.rotate_camera(-1 * camera_speed, 0, 0);
		break;
	case  SDLK_LEFT:
		c.rotate_camera(0, camera_speed, 0);
		break;
	case SDLK_RIGHT:
		c.rotate_camera(0, -1 * camera_speed, 0);
		break;
	default: break;
	}
}








