#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/glm.hpp> 
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

using namespace glm;

class Camera
{
	glm::vec3 position;
	glm::vec3 viewDirection;
	const glm::vec3 Up;
	glm::vec2 oldMousePosition;
public:
	Camera();
	//~Camera();

	void mouseUpdate( const glm::vec2 &newMousePosition);
	void keyboardUpdate(GLFWwindow *window);

	glm::mat4 getWorldToViewMatrix() const;
	void SetOldMousePosition(const glm::vec2 &newMousePosition);
	void UpdateCamera(GLFWwindow* window);
 
	bool m_clickedMouseRight = false;

};

