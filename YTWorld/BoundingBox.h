#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp>
 #include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "Geometry\Shader.h"
class BoundingBox
{
public:
	BoundingBox(glm::vec3 _min, glm::vec3 _max, float width = 3.0f);
	void drawBox(glm::mat4 _fuuMatrix);
	~BoundingBox();

	glm::vec3 m_min;
	glm::vec3 m_max;
	glm::vec3 m_center;
	glm::vec3 m_size;
	glm::mat4 m_transform;  

	GLuint vbo_vertices;
	GLuint ibo_elements;

	Shader *m_shader;

	float m_width;

	static Shader *s_shader;

	void PrintInfo();

};

