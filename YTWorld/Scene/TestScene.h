#pragma once
#include "Scene.h"
#include "..\GameObject.h"
#include "..\GameObjects\GroundObject.h"
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/glm.hpp> 
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace glm;

class TestScene :public IScene
{
public:
	TestScene(GLFWwindow *_window, Camera *_camera);
	~TestScene();
	GLFWwindow* m_window;
	Camera *m_camera;
	void BeginScene() override;
	void UpdateScene(float deltaTime) override;
	void EndScene() override;
	void keyboardUpdate(GLFWwindow *window, GameObject *bindObject, float deltaTime);

};

