
#include "Camera.h"
#include <glm\gtx\\transform.hpp>
#include <iostream>

float mouseSpeed = 0.005f;
float speed =3.0f; // 3 units / second
Camera::Camera() : viewDirection(0.0f, 0.0f, -1.0f), Up(0.0f, 1.0f, 0.0f), oldMousePosition(glm::vec2(320,240))
{
	position = glm::vec3(0, 10.0f, 5.0f);
}

void Camera::mouseUpdate(const glm::vec2 &newMousePosition)
{
	glm::vec2 mouseDelta = newMousePosition - oldMousePosition;
	/*
	//horizon
	viewDirection = glm::mat3(glm::rotate(-mouseDelta.x * mouseSpeed, Up))* viewDirection;
	//vertical
	glm::vec3 toRotateAround = glm::cross(viewDirection, Up);
	viewDirection = glm::mat3(glm::rotate(-mouseDelta.y * mouseSpeed, toRotateAround))* viewDirection;
	*/
	//revise the code abrove
	glm::vec3 toRotateAround = glm::cross(viewDirection, Up);  //right vector
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * mouseSpeed, Up)*
		glm::rotate(-mouseDelta.y * mouseSpeed, toRotateAround);

	viewDirection = glm::mat3(rotator) * viewDirection;

	glm::vec3 camUp = glm::cross(toRotateAround,viewDirection);  //right vector
//	std::cout << "x " << camUp.x << "y " << camUp.y << "z " << camUp.z << std::endl;
	oldMousePosition = newMousePosition;
 
}

void Camera::keyboardUpdate(GLFWwindow *window)
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	glm::vec3 right = glm::cross(viewDirection, Up);  //right vector

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		position += viewDirection * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		position -= viewDirection * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, Up);

}

//Camera::~Camera()
//{
//}
