#pragma once

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp> 
#include "../CommonFunction/ObjLoader.h"
 


class Mesh
{
public:
	Mesh(){}
	Mesh(std::map<std::string, Mesh> &_existList, const char* objFilePath);
	~Mesh();

	void	InitMesh(const char* objFilePath);
	void  Draw();

	//member
	std::map<std::string, Mesh> existList;
	GLuint m_vertexArrayObjID;
	GLuint	m_numVertices;
	GLuint	m_numCoords;
	GLuint	m_numNormals;

	glm::vec3 min;
	glm::vec3 max;
 
};

