#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp> 
#include "../CommonFunction/DDSLoader.h"
class Material
{
public:
	Material(const char* ddsFilePath);
	~Material();
	void InitTexture(const char* ddsFilePath);
	void Bind();

	//member
	GLuint m_TextureID;
};

