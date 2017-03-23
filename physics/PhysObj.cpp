#include "PhysObj.h"



//PhysObj functions
PhysObj::PhysObj()
{

}


PhysObj::PhysObj(float pos_x, float pos_y, float m, float width, float height, float step)
{
	position = glm::vec2(pos_x, pos_y);

	mass.mass = m;
	if (m == 0)
		mass.inv_mass = 0;
	else
		mass.inv_mass = 1 / mass.mass;

	shape = Collider(pos_x, pos_y, width, height);

	time_step = step;
	original_time = step;

	static_friction = 1.0f;
	dynamic_friction = 1.0f;
}

PhysObj::PhysObj(float pos_x, float pos_y, float m, float radius, float step)
{
	position = glm::vec2(pos_x, pos_y);

	mass.mass = m;
	if (m == 0)
		mass.inv_mass = 0;
	else
		mass.inv_mass = 1 / mass.mass;

	shape = Collider(pos_x, pos_y, radius);

	time_step = step;
	original_time = step;

	static_friction = 1.0f;
	dynamic_friction = 1.0f;
}

void PhysObj::add_force(float fx, float fy)
{
	force +=  glm::vec2(fx, fy);
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

/*!
	Based on tutorial from
	https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-friction-scene-and-jump-table--gamedev-7756

*/
inline void resolve_friction(Manifold& m, float e) {
	

	// Re-calculate relative velocity after normal impulse
	// is applied (impulse from first article, this code comes
	// directly thereafter in the same resolve function)
	glm::vec2 rv = m.B->velocity - m.A->velocity;

	// Solve for the tangent vector
	auto t_pre = rv - (glm::dot(rv, m.normal) * m.normal);
	glm::vec2 t = (!t_pre[0] && !t_pre[1]) ? glm::vec2(0.0f,0.0f) : glm::normalize(t_pre);

	float j = -(1 + e) * glm::dot(t, rv);
	j /= (m.A->mass.inv_mass + m.B->mass.inv_mass);


		// Solve for magnitude to apply along the friction vector
	float jt = -glm::dot(rv, t);
	jt = jt / (m.A->mass.inv_mass + m.B->mass.inv_mass);

	// PythagoreanSolve = A^2 + B^2 = C^2, solving for C given A and B
	// Use to approximate mu given friction coefficients of each body
	float mu = pythagorean_solve(m.A->static_friction, m.B->static_friction);

	// Clamp magnitude of friction and create impulse vector
	glm::vec2 friction_impulse;
	if (abs(jt) < j * mu)
		friction_impulse = jt * t;
	else
	{
		friction_impulse = -j * t * pythagorean_solve(m.A->dynamic_friction, m.B->dynamic_friction);
	}

	if (friction_impulse[0] > 5 || friction_impulse[1] > 5)
		std::cout << "Too much friction_impulse!";

	// Apply
	m.A->velocity -= (m.A->mass.inv_mass) * friction_impulse;
	m.B->velocity -= (m.B->mass.inv_mass) * friction_impulse;
};

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

	if (m.normal.x == 0) //affecting y direction
	{
		float temp = m.A->acceleration.x;
		m.A->acceleration = glm::vec2(temp, 0);
	}
	else				//affecting x direction
	{
		float temp = m.A->acceleration.y;
		m.A->acceleration = glm::vec2(0, temp);
	}

	//float e = std::min(m.A->material.restitution, m.B->material.restitution);

	//float denom = (m.A->mass.mass + m.B->mass.mass);

	//std::cout << "penetration: " << m.penetration << "\nNormal: " << m.normal.x << " " << m.normal.y << "\n";

	//if (m.normal.x == 0) //affecting y direction
	//{
	//	float temp = m.A->acceleration.x;
	//	m.A->acceleration = glm::vec2(temp, 0);
	//}
	//else				//affecting x direction
	//{
	//	float temp = m.A->acceleration.y;
	//	m.A->acceleration = glm::vec2(0, temp);
	//}

	///*m.A->acceleration = glm::vec2(0, 0);
	//m.B->acceleration = glm::vec2(0, 0);*/

	//m.A->velocity = e * ( ((m.A->mass.mass - m.B->mass.mass) / denom)*m.A->velocity + ((2 * m.A->mass.mass) / denom)*m.B->velocity); 

	//m.B->velocity = e * ( ((m.B->mass.mass - m.A->mass.mass) / denom)*m.A->velocity + ((2 * m.A->mass.mass) / denom)*m.A->velocity);

	resolve_friction(m, e);
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
	std::cout << "x / y; " << position.x << " " << position.y << "\n";
	std::cout << "velocity: " << velocity.x << " " << velocity.y << "\n";
	std::cout << "time step: " << time_step << "\n";
	std::cout << "mass: " << mass.mass << "\n";
	std::cout << "forces: " << force.x << " " << force.y << "\n";
	std::cout << "accel: " << acceleration.x << " " << acceleration.y << "\n";
	std::cout << "\n\n";

}

bool Collide(Manifold& m)
{
	if (m.A->shape.type == 1 && m.B->shape.type == 1)			//both are rects
		return AABB_vs_AABB(m);

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
					m.normal = glm::vec2(-1, 0);
				else
					m.normal = glm::vec2(1, 0);

				m.penetration = x_overlap;
				return true;
			}
			else									// y has the least
			{
				if (n.y < 0)
					m.normal = glm::vec2(0, -1);
				else
					m.normal = glm::vec2(0, 1);

				m.penetration = y_overlap;
				return true;
			}
		}
	}
	return false;
}
