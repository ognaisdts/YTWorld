#include "RigidBody.h"



RigidBody::RigidBody(float _mass, float _coe)
{
	m_velocity = vec3(0, 0, 0);
	m_accelerate = vec3(0, 0, 0);
	m_force = vec3(0, 0, 0);
	m_mass = _mass;
	m_coe = _coe;
}


RigidBody::~RigidBody()
{
}
