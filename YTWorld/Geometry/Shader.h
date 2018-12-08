#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <glm/glm.hpp> 
#include <string>
 
 
class Shader
{
public:
	Shader(const char * vertex_file_path, const char * fragment_file_path);
	~Shader();
	void InitShader(const char * vertex_file_path, const char * fragment_file_path);
	void UpdateShader(const glm::mat4& _fullTransformMatrix );
	void UpdateTextureShader();
	void Bind();

	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	GLuint m_ProgramID;
	GLuint m_uniforms[2]; //might extend
};

