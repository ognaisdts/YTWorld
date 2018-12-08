#pragma once
#include "../GameObject.h"
class GroundObject : public GameObject
{
public:
	GroundObject(const char* objfile,
		const char* ddsfile,
		const char* vertexShader,
		const char* fragmentShader,
		std::map<std::string, Mesh> &existList,
		vec3 _position,
		float angle = 0.0f) : GameObject(objfile,
		ddsfile,
		vertexShader,
		fragmentShader,
		existList,
		_position,
		angle)
	{
		m_mass = 1000000;
	
	}

	~GroundObject();

	  virtual void Update(float deltaTime) override;
};

