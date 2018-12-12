#pragma once
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <set>
#include <vector>
#include <string>
#include "Geometry\Mesh.h"
#include "Geometry\Material.h"
#include "Geometry\Shader.h"
#include "Camera.h"
#include "BoundingBox.h"
#include "CollisionDetection\CollisionDetection.h"
#include "CollisionDetection\OcTreeNode.h"
#include "Render\RenderComponent.h"
#include "Physic\RigidBody.h"

using namespace glm;
using namespace std;

class OcTreeNode;

class GameObject
{
public:
	//GameObject();

	GameObject(
		vec3 _position,
		float angle =0
		);



	~GameObject();

	//geo info
	glm::vec3 position;
	glm::mat4 modelToWorldMatrix;
	float m_angle;

	glm::mat4 projectionMatrix;
	mat4 translateMatrix; //= glm::translate(vec3(0.0f, 0.0f, -3.0f));
	mat4 rotationMatrix;// = glm::rotate(angle, vec3(1.0f, 0.0f, 0.0f));   // include gtx\transform
	RenderComponent *m_renderComponent;

	//geomethod
	glm::mat4 getModelToWorldMatrix();
	void setModelMatrix( glm::mat4 rotationM, glm::mat4 translateM  );
	//openGL method and member



	void CreateBoundingBox();
	void AddRenderComponent(const char* objFilePath, const char* ddsFilePath, const char* vertexShader, const char* fragmentShader, bool isSky = false);
	void AddRigidBody(float _mass = 1.0f, float _coe = 0);

	//OcTree
	virtual void Update(float deltaTime); // current for oct 
	OcTreeNode *m_owner;
	void refreshOcTree();



	BoundingBox *m_obb;

	mat4 m_fullMatrix;

	//// for ocTree 
	vec3 m_prevPos;


	// physic data
	RigidBody *m_rigid;
	bool isJump;


};

