#include "PhysObj.h"

//PhysObj functions
PhysObj::PhysObj()
{

}

PhysObj::PhysObj(float pos_x, float pos_y, float m, float width, float height, float step)
	: time_step(step), original_time(step), shape(Collider(pos_x, pos_y, width, height)), position(glm::vec2(pos_x, pos_y))
{
	mass.mass = m;
	if (m == 0)
		mass.inv_mass = 0;
	else
		mass.inv_mass = 1 / mass.mass;

}

PhysObj::PhysObj(float pos_x, float pos_y, float m, float radius, float step)
	: time_step(step), original_time(step), shape(Collider(pos_x, pos_y, radius)), position(glm::vec2(pos_x, pos_y))
{
	mass.mass = m;
	if (m == 0)
		mass.inv_mass = 0;
	else
		mass.inv_mass = 1 / mass.mass;
}

void PhysObj::add_force(float fx, float fy)
{
	force += glm::vec2(fx, fy);
}

void PhysObj::reset_object()
{
	force = glm::vec2(0, 0);
	acceleration = glm::vec2(0, 0);
	velocity = glm::vec2(0, 0);
}

void PhysObj::reset_force(float x, float y)
{
	force = glm::vec2(x, y);
}

void PhysObj::add_gravity(glm::vec2 gravity)
{
	force = mass.mass * gravity;
}

void PhysObj::calculate_positon()
{
	if (mass.mass == 0)
	{
		acceleration = glm::vec2(0.0, 0.0);
		velocity = glm::vec2(0.0, 0.0);

	}
	else
	{
		glm::vec2 last_acceleration = acceleration;
		position += velocity * time_step + (0.5f * last_acceleration * (time_step * time_step));
		acceleration = force / mass.mass;
		glm::vec2 avg_acceleration = (last_acceleration + acceleration) / 2.0f;
		velocity += avg_acceleration * time_step;
	}

	shape.position_correction(position);
}

float pythagorean_solve(float a, float b) {
	return glm::sqrt((a*a) + (b*b));
}

inline void resolve_friction(Manifold& m, float e, float j) {

	// Re-calculate relative velocity after normal impulse
	// is applied (impulse from first article, this code comes
	// directly thereafter in the same resolve function)
	glm::vec2 rv = m.B->velocity - m.A->velocity;

	// Solve for the tangent vector
	auto t_pre = rv - (glm::dot(rv, m.normal) * m.normal);
	glm::vec2 t = (!t_pre[0] && !t_pre[1]) ? glm::vec2(0.0f, 0.0f) : glm::normalize(t_pre);

	// Solve for magnitude to apply along the friction vector
	float jt = glm::dot(rv, -t);			// bug was here, there was an unneeded /- sign before the dot
	jt = jt / (m.A->mass.inv_mass + m.B->mass.inv_mass);

	// PythagoreanSolve = A^2 + B^2 = C^2, solving for C given A and B
	// Use to approximate mu given friction coefficients of each body
	float mu = std::min(m.A->static_friction, m.B->static_friction);

	// Clamp magnitude of friction and create impulse vector
	glm::vec2 friction_impulse;
	if (abs(jt) < j * mu)
		friction_impulse = jt * t; // jt * t;
	else
	{
		friction_impulse = -j * t * std::min(m.A->dynamic_friction, m.B->dynamic_friction);
	}

	if (friction_impulse[0] > 5 || friction_impulse[1] > 5)
		std::cout << "Too much friction_impulse!";

	// Apply
	m.A->velocity -= (m.A->mass.inv_mass) * friction_impulse;
	m.B->velocity += (m.B->mass.inv_mass) * friction_impulse;
}


void add_obj_id(Manifold & m)
{
	if (std::find(m.A->collided_with.begin(), m.A->collided_with.end(), m.B->entity_ID) == m.A->collided_with.end()) {
		m.A->collided_with.push_back(m.B->entity_ID);
	}
}

/*!
Based on tutorial from
https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-friction-scene-and-jump-table--gamedev-7756

*/
void calculate_resolution(Manifold& m)
{
	glm::vec2 rv = m.B->velocity - m.A->velocity;

	float vel_on_norm = glm::dot(m.normal, rv);

	if (vel_on_norm > 0)
		return;

	float e = std::min(m.A->material.restitution, m.B->material.restitution);

	float j = -(1 + e) * vel_on_norm;
	j /= (m.A->mass.inv_mass + m.B->mass.inv_mass);

	glm::vec2 impulse = j * m.normal;

	float sum_mass;

	if (m.A->mass.mass == 0)
		sum_mass = 2 * m.B->mass.mass;
	else if (m.B->mass.mass == 0)
		sum_mass = 2 * m.A->mass.mass;
	else
		sum_mass = m.A->mass.mass + m.B->mass.mass;

	float ratio = m.A->mass.mass / sum_mass;
	m.A->velocity -= ratio * impulse;

	ratio = m.B->mass.mass / sum_mass;
	m.B->velocity += ratio * impulse;


	// Stop acceleration when come in contact with an object
	//if (m.normal.x == 0 || m.normal.y ==0) //affecting y direction
	//{
	//	float temp = m.A->acceleration.x;
	//	m.A->acceleration = glm::vec2(temp, 0);
	//}
	//else				//affecting x direction
	//{
	//	float temp = m.A->acceleration.y;
	//	m.A->acceleration = glm::vec2(0, temp);
	//}

	resolve_friction(m, e, j);
}


void sink_correction(Manifold & m)
{
	const float percent = 0.2f;
	const float slop = 0.01f;
	glm::vec2 correction = std::max(m.penetration - slop, 0.0f) / (m.A->mass.inv_mass + m.B->mass.inv_mass) * percent * m.normal;
	m.A->position -= m.A->mass.inv_mass * correction;
	m.B->position += m.B->mass.inv_mass * correction;
}

//-------Utility functions-------
void PhysObj::print_out_info()
{
	std::ofstream myfile;
	myfile.open("physics_log.txt", std::fstream::in | std::fstream::app);
	myfile << "x / y; " << position.x << " " << position.y << "\n";
	myfile << "velocity: " << velocity.x << " " << velocity.y << "\n";
	myfile << "time step: " << time_step << "\n";
	myfile << "mass: " << mass.mass << "\n";
	myfile << "forces: " << force.x << " " << force.y << "\n";
	myfile << "accel: " << acceleration.x << " " << acceleration.y << "\n";
	myfile << "\n\n";

}

//------Collision Functions------
bool Collide(Manifold& m)
{
	if (m.A->shape.type == 1 && m.B->shape.type == 1)			//both are rects
		return AABB_vs_AABB(m);
	else if (m.A->shape.type == 1 && m.B->shape.type == 0)
		return AABB_vs_Circle(m);
	return false;
}

bool AABB_vs_AABB_UnO(Manifold& m)
{
	//checks to see if most basic of intersection is happening
	if (m.A->shape.get_coll().Rect.max.x <= m.B->shape.get_coll().Rect.min.x || m.A->shape.get_coll().Rect.min.x >= m.B->shape.get_coll().Rect.max.x) return false;
	else if (m.A->shape.get_coll().Rect.max.y <= m.B->shape.get_coll().Rect.min.y || m.A->shape.get_coll().Rect.min.y >= m.B->shape.get_coll().Rect.max.y) return false;
	else //if an intersection then run heavy code
		return AABB_vs_AABB_UnO(m);
}

//solutions?
// http://www.metanetsoftware.com/technique/tutorialA.html#section3
//
bool AABB_vs_Circle(Manifold& m)
{
	return false;
}

bool AABB_vs_AABB(Manifold& m)
{
	PhysObj *A = m.A;
	PhysObj *B = m.B;

	glm::vec2 n = B->position - A->position;

	AABB abox = A->shape.get_coll().Rect;
	AABB bbox = B->shape.get_coll().Rect;

	//std::cout << "Abox: " << abox.min.x << " " << abox.min.y << " " << abox.max.x << " " << abox.max.y << "\n";
	//std::cout << "Bbox: " << bbox.min.x << " " << bbox.min.y << " " << bbox.max.x << " " << bbox.max.y << "\n";

	//for the x axis
	float a_half = (abox.max.x - abox.min.x) / 2;
	float b_half = (bbox.max.x - bbox.min.x) / 2;

	float x_overlap = a_half + b_half - abs(n.x);

	glm::vec2 temp_norm; 
	//Separation Axis theorem
	if (x_overlap > 0)
	{
		a_half = (abox.max.y - abox.min.y) / 2;
		b_half = (bbox.max.y - bbox.min.y) / 2;

		float y_overlap = a_half + b_half - abs(n.y);

		if (y_overlap > 0)
		{
			//find which axis has least penetration
			if (x_overlap < y_overlap)				// x has the least
			{
				if (n.x < 0)
				{
					temp_norm = glm::vec2(-1, 0);
					m.normal = temp_norm;
					A->normals_acting.push_back(temp_norm);
				}
				else
				{
					temp_norm = glm::vec2(1, 0);
					m.normal = temp_norm;
					A->normals_acting.push_back(temp_norm);
				}

				m.penetration = x_overlap;
				add_obj_id(m);
				return true;
			}
			else									// y has the least
			{
				if (n.y < 0)
				{
					temp_norm = glm::vec2(0, -1);
					m.normal = temp_norm;
					A->normals_acting.push_back(temp_norm);
				}
				else
				{
					temp_norm = glm::vec2(0, 1);
					m.normal = temp_norm;
					A->normals_acting.push_back(temp_norm);
				}


				m.penetration = y_overlap;
				add_obj_id(m);
				return true;
			}
		}
	}
	return false;
}

