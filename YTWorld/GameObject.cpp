#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include "GameObject.h"
#include <glm/glm.hpp> 
#include "CommonFunction\ObjLoader.h"
#include "CommonFunction\DDSLoader.h"
#include "Render\RenderManager.h"

using namespace glm;
using namespace std;
//GameObject::GameObject()
//	  : position(vec3(0, 0, 0)),
//		rotationMatrix(glm::rotate(0.0f, vec3(0, 1, 0))),
//		translateMatrix(glm::translate(vec3(0, 0, 0)))
//{
//
//}
GameObject::GameObject(
				vec3 _position,
				float angle
	)
	: position(_position),
	m_prevPos(_position),
	 rotationMatrix(glm::rotate(0.0f, vec3(0,1,0))),
	 translateMatrix(glm::translate(_position)),
	 m_obb(nullptr),
	 m_angle(angle)
{
	isJump = false;
	projectionMatrix = glm::perspective(glm::radians(67.0f), 640.f / 480.0f, 0.3f, 1000.0f);
 	//InitGLdata(objfile, ddsfile, vertexShader, fragmentShader);

}

void GameObject::CreateBoundingBox()
{
	//if (m_mesh != nullptr && m_obb == nullptr)
	if(m_renderComponent!= nullptr)
	{
		m_obb = new BoundingBox(m_renderComponent->m_mesh->min, m_renderComponent->m_mesh->max);
	}
 
}

void GameObject::AddRenderComponent(const char* objFilePath, const char* ddsFilePath, const char* vertexShader, const char* fragmentShader, bool isSky)
{
	if (m_renderComponent == nullptr)
	{
		m_renderComponent = new RenderComponent(objFilePath, ddsFilePath, vertexShader, fragmentShader, isSky);
		RenderManager::RegisterGameObject(this);
	}
}

void GameObject::AddRigidBody(float _mass, float _coe)
{
	if (m_rigid == nullptr)
	{
		m_rigid = new RigidBody(_mass, _coe);
	}
}

void GameObject::setModelMatrix(glm::mat4 rotationM, glm::mat4 translateM)
{
	translateMatrix = translateM;
	rotationMatrix = rotationM;
}
//void GameObject::setWorldToViewMatrix(glm::mat4 _worldToViewMatrix)
//{
//	worldToViewMatrix = _worldToViewMatrix;
//}
glm::mat4 GameObject::getModelToWorldMatrix()
{
	return translateMatrix*rotationMatrix;

}


 

void GameObject::refreshOcTree()
{
	if (m_owner->parent == nullptr &&  m_owner->IsObjInsideBox(this, m_owner->m_region))
	{

		OcTreeNode *preOwner = m_owner;

		auto it = find(preOwner->m_objects.begin(), preOwner->m_objects.end(), this);
		if (it != preOwner->m_objects.end())
		{
			preOwner->m_objects.erase(it);
		}
		m_owner->InsertInExistNode(this);

		return;
	}
	if (m_owner->parent != nullptr && m_owner != nullptr) // not a root
	{
		if (m_owner->IsObjInsideBox(this, m_owner->m_region))
		{
			OcTreeNode *preOwner = m_owner;
			m_owner->InsertInExistNode(this);

			auto it = find(preOwner->m_objects.begin(), preOwner->m_objects.end(), this);
			if (it != preOwner->m_objects.end())
			{
				preOwner->m_objects.erase(it);
			}

			return;
		}
		else
		{
			OcTreeNode * P = m_owner->parent;
			while (!(P->IsObjInsideBox(this, P->m_region)) && P)
			{
				P = P->parent;
			}

			OcTreeNode *preOwner = m_owner;

			if (P == nullptr)
			{
				OcTreeNode::getRootInstance()->InsertObjects(this);
			}
			else
			{
				P->InsertObjects(this);
			}

			auto it = find(preOwner->m_objects.begin(), preOwner->m_objects.end(), this);
			if (it != preOwner->m_objects.end())
			{
				preOwner->m_objects.erase(it);
				OcTreeNode::KillNode(preOwner, this);
			}
		}
	}
}

void GameObject::Update(float deltaTime)
{
	if (m_rigid)
	{
		m_rigid->m_force = vec3(0, -10, 0) * m_rigid->m_mass;

		// position update
		m_rigid->m_accelerate = m_rigid->m_force / m_rigid->m_mass;
		m_rigid->m_velocity += (m_rigid->m_accelerate*deltaTime);
		position += (m_rigid->m_velocity * deltaTime);

		if (position.y < 0.6f - 0.002f)
		{
			position.y = 0.6f + 0.002f;
			m_rigid->m_velocity = vec3(0, 0, 0);
			m_rigid->m_force = vec3(0, 0, 0);
		}
	}



	translateMatrix = translate(position);

	if ( m_prevPos != position)
	{
		m_prevPos = this->position;

		refreshOcTree();
	}
 
}




GameObject::~GameObject()
{

}
