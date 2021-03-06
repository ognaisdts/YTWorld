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
	Mesh(const char* objFilePath);
	~Mesh();

	void  InitMesh(const char* objFilePath);
	void  Draw();
	static std::map<std::string, Mesh> existList;//obj name, vertex array ID,  numVertice

	//member
	GLuint m_vertexArrayObjID;
	GLuint	m_numVertices;
	GLuint	m_numCoords;
	GLuint	m_numNormals;

	glm::vec3 min;
	glm::vec3 max;
 
};


