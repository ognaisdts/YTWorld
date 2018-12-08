#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/glm.hpp> 
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include "Camera.h"

using namespace std;
using namespace glm;

class MousePicker
{
public:
	MousePicker(Camera &_cam, mat4 _projectionMatrix, GLFWwindow* _window);
	~MousePicker();
	vec3 getCurrentRay(){ return m_currentRay; }
	void update(Camera &_cam);
	vec3 calcualteRay();
	vec2 getNDC(double x, double y);
	vec4 toEyeCoord(vec4 _clipCoord);
	vec3 toWorldCoodrs(vec4 eyeCoord);

private:
	//member
	mat4 m_projectionMatrix;
	mat4 m_viewMatrix;
	Camera m_camera;
	vec3 m_currentRay;
	GLFWwindow *window;
};

