#pragma once
#include "../GameObject.h"
class GroundObject : public GameObject
{
public:
	GroundObject(vec3 _position, float angle = 0.0f) : GameObject(_position, angle)
	{
		AddRigidBody(1000000);
	}

	~GroundObject();

	  virtual void Update(float deltaTime) override;
};

