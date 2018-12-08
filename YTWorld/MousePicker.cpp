#include "MousePicker.h"


MousePicker::MousePicker(Camera &_cam, mat4 _projectionMatrix, GLFWwindow *_window)
{
	this->window = _window;
	//this->m_camera = _cam;
	this->m_projectionMatrix = _projectionMatrix;
	this->m_viewMatrix = _cam.getWorldToViewMatrix();
}

void MousePicker::update(Camera &_cam)
{
	m_viewMatrix = _cam.getWorldToViewMatrix();
	m_currentRay = calcualteRay();
}

vec3 MousePicker::calcualteRay()
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	vec2 normalNDC = getNDC(xpos,ypos);
	vec4 clipCoords(normalNDC.x, normalNDC.y, -1, 1 );
	vec4 eyeCoords = toEyeCoord(clipCoords);
	vec3 worldRay = toWorldCoodrs(eyeCoords);

	return worldRay;
}
vec4 MousePicker::toEyeCoord(vec4 _clipCoord)
{
	mat4 inversProjection = glm::inverse(m_projectionMatrix);
	vec4 eyeCoord = inversProjection * _clipCoord;
 
	return vec4(eyeCoord.x, eyeCoord.y, -1, 0);
}

vec3 MousePicker::toWorldCoodrs(vec4 eyeCoord)
{
	mat4 inverseView = inverse(m_viewMatrix);
	vec4 rayWorld = inverseView * eyeCoord;
	vec3 mouseRay(rayWorld);
	normalize(mouseRay);

	return mouseRay;
}


vec2 MousePicker::getNDC(double _x, double _y)
{
	double x = (2 * _x) / 640 - 1;
	double y = 2 * _y / 480 - 1;

	return vec2(x, y);
}



MousePicker::~MousePicker()
{
}
