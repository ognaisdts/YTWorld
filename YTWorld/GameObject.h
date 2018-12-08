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

using namespace glm;
using namespace std;

class OcTreeNode;

class GameObject
{
public:
	//GameObject();

	GameObject(
		const char* objfile, 
		const char* ddsfile, 
		const char* vertexShader,
		const char* fragmentShader,
		std::map<std::string, Mesh> &existList,
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


	//geomethod
	glm::mat4 getModelToWorldMatrix();
	void setModelMatrix( glm::mat4 rotationM, glm::mat4 translateM  );
	//void setWorldToViewMatrix( glm::mat4 _worldToViewMatrix );
	//openGL method and member

	void InitGLdata(const char* objFilePath, const char* ddsFilePath, const char* vertexShader, const char* fragmentShader, std::map<std::string, Mesh> &_existList);
	void createMesh(const char* ddsFilePath, std::map<std::string, Mesh> &_existList);
	void createShader(const char* vertexShader, const char* fragmentShader);
	void createMaterial(const char* ddsFilePath);

	void createBoundingBox();

	void Draw( glm::mat4 worldToViewMatrix );

	//OcTree
	virtual void Update(float deltaTime); // current for oct 
	set<OcTreeNode*> ownerNodes; // which node are this obj inside
	void refreshOwner();
	OcTreeNode *m_owner;
	void refreshOcTree();

	Mesh *m_mesh;
	Shader *m_shader;
	Material *m_material;
 
	GLuint Texture;

	BoundingBox *m_obb;

	mat4 m_fullMatrix;

	//// for ocTree 
	vec3 m_prevPos;


	// physic data
	  glm::vec3 m_velocity;
	  float m_mass;
	  glm::vec3 m_accelerate;
	  glm::vec3 m_force;
	  bool isJump;

	  void addFroceToOther(GameObject *other);

	  float m_coe;


	//std::map<std::string, std::vector<GLuint>> m_existList;

	//// openGL method
	//void RegisterObjMesh(const char* objfile, const char* ddsfile, std::map<std::string, std::vector<GLuint>> &existList);
	//void setShaderProgram(GLuint newShaderProgramID, std::map<std::string, std::vector<GLuint>> &existList);
	//void  Draw(glm::mat4 viewMatrix, mat4 projectionMatrix);
	//// openGL member
	//GLuint MeshVertexArrayID;
	//GLuint numVertices;
	//GLuint Texture;
	//GLuint ShaderProgramID;
};

