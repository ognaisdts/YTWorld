#pragma once
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "glm\glm.hpp"

using namespace glm;

class RigidBody
{
public:
	RigidBody(float _mass, float _coe);
	~RigidBody();

	

	//class members

	// physic data
	glm::vec3 m_velocity;
	float m_mass;
	glm::vec3 m_accelerate;
	glm::vec3 m_force;
	float m_coe;
};

