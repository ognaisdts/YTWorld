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
#include "Primitives\Vertex.h"
#include "Primitives\ShapeGenerator.h"
#include "Camera.h"
#include "CollisionDetection\OcTreeNode.h"
#include "CollisionDetection\CollisionDetection.h"
#include "GameObject.h"
#include "GameObjects\GroundObject.h"
#include "MousePicker.h"
#include "Render\RenderManager.h"
#include "Scene\Scene.h"
#include "Scene\TestScene.h"

using namespace glm;
using namespace std;

 
std::map<string, Mesh>  objList; //obj name, vertex array ID,  numVertice
mat4 projectionMatrix = glm::perspective(glm::radians(67.0f), 640.f / 480.0f, 0.3f, 1000.0f);
 
// glfwGetTime is called only once, the first time this function is called
static double lt;// = glfwGetTime();

// Compute time difference between current and last frame
double currentTime;// = glfwGetTime();
float deltaTime;// = float(currentTime - lt);

 



int main(void)
{
	GLFWwindow* window;

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(640, 480, "YT World", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 640 / 2, 480 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	Camera *camera = new Camera();

	RenderManager *renderManager = new RenderManager(camera);
	IScene *curScene = new TestScene(window, camera);

	curScene->BeginScene();


	MousePicker *mousePicker = new MousePicker(*camera, projectionMatrix, window);
	// OcTreeNode::getRootInstance()->ProcessItem(&box2);
	//GameObject *bindObject = box1;
	bool isTabPress = false;
	// glfwGetTime is called only once, the first time this function is called
	lt = glfwGetTime();

 
 	do{

		// Compute time difference between current and last frame
		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lt);


		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mousePicker->update(*camera);
		vec3 test = mousePicker->getCurrentRay();
		//std::cout << "x " << test.x << "y " << test.y << "z " << test.z << std::endl;


		camera->UpdateCamera(window);
		curScene->UpdateScene(deltaTime);
 		CollisionDetection::processCollisionRespond(OcTreeNode::getRootInstance()->GetIntersectionList(vector<GameObject*>())  , deltaTime);
		renderManager->Render();

		//draw octree
		OcTreeNode::drawDebugBox(OcTreeNode::getRootInstance(), projectionMatrix * camera->getWorldToViewMatrix());

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


		lt = currentTime;

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

 
	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
