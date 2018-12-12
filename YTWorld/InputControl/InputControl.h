#pragma once
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
class InputControl
{
public:


	InputControl(GLFWwindow* window)
	{
		m_window = window;
	}

	void Update();

private:
	GLFWwindow* m_window;
	~InputControl();

};

