#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include "GameObject.h"
#include <glm/glm.hpp> 
#include "CommonFunction\ObjLoader.h"
#include "CommonFunction\DDSLoader.h"

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
				const char* objfile,
				const char* ddsfile,
				const char* vertexShader,
				const char* fragmentShader,
				std::map<std::string, Mesh> &existList,
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
	m_velocity = vec3(0, 0, 0);
	m_accelerate = vec3(0, 0, 0);
	m_force = vec3(0, 0, 0);
	m_mass = 1.0f;
	m_coe = 0;
	isJump = false;
	projectionMatrix = glm::perspective(glm::radians(67.0f), 640.f / 480.0f, 0.3f, 1000.0f);
 	InitGLdata(objfile, ddsfile, vertexShader, fragmentShader, existList);

}

void GameObject::InitGLdata(const char* objFilePath, const char* ddsFilePath, const char* vertexShader, const char* fragmentShader, std::map<std::string, Mesh> &_existList)
{
	createShader(vertexShader, fragmentShader);
	createMesh(objFilePath, _existList);
 
	createMaterial(ddsFilePath);

	glBindVertexArray(0);

}

void GameObject::createMesh(const char* objFilePath, std::map<std::string, Mesh> &_existList)
{
	if (m_mesh == nullptr)
		m_mesh = new Mesh(_existList, objFilePath);
}
void GameObject::createShader(const char* vertexShader, const char* fragmentShader)
{
	if (m_shader==nullptr)
		m_shader = new Shader(vertexShader, fragmentShader);
}
void GameObject::createMaterial(const char* ddsFilePath )
{
	if (m_material == nullptr)
		m_material = new Material(ddsFilePath);
}
void GameObject::createBoundingBox()
{
	if (m_mesh != nullptr && m_obb == nullptr)
	{
		m_obb = new BoundingBox(m_mesh->min, m_mesh->max);
	}
 
}



void GameObject::Draw(glm::mat4 worldToViewMatrix)
{


	if (m_mesh == nullptr) return;
	  m_fullMatrix = projectionMatrix*worldToViewMatrix* this->getModelToWorldMatrix();

	//////// draw bounding box

	if (m_shader != nullptr)
	{
		m_shader->Bind();
		
		m_shader->UpdateShader(m_fullMatrix);
	}
	if (m_material != nullptr)
	{
		m_material->Bind();

	}
  	m_mesh->Draw();
 
//	if (m_obb)
	//	m_obb->drawBox(fullMatrix);

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
 	m_force = vec3(0, -10, 0) * m_mass;

	// position update
	m_accelerate = m_force / m_mass;
	m_velocity += (m_accelerate*deltaTime);
	position += (m_velocity * deltaTime);

	if (position.y < 0.6f - 0.002f)
	{
		position.y = 0.6f + 0.002f;
		m_velocity = vec3(0, 0, 0);

	}



	translateMatrix = translate(position);

	if (    m_prevPos != position)
	{
		m_prevPos = this->position;


		refreshOcTree();
 


	}
 
 
	m_force = vec3(0, 0, 0);
}

void GameObject::addFroceToOther(GameObject *other)
{
	//other->m_force = this->m_force;
	//other->
}


GameObject::~GameObject()
{
	delete m_material;
	delete m_mesh;
	delete m_shader;
}
