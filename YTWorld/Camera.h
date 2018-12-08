#pragma once
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm\glm.hpp>

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

 
};

