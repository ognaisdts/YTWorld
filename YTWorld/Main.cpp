#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
GLFWwindow* window;
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



using namespace glm;
using namespace std;

 
Camera camera;
std::map<string, Mesh>  objList; //obj name, vertex array ID,  numVertice
mat4 projectionMatrix = glm::perspective(glm::radians(67.0f), 640.f / 480.0f, 0.3f, 1000.0f);
 
// glfwGetTime is called only once, the first time this function is called
static double lt;// = glfwGetTime();

// Compute time difference between current and last frame
double currentTime;// = glfwGetTime();
float deltaTime;// = float(currentTime - lt);

 
//double sum(int a, int b)
//{
//	return double(a + b);
//}
//int  sum(int a, int b)
//{
//	return  int (a);
//}


double sum(double a)
{
	return a;
}
int sum(int a)
{
	return a;
}

  class Base
{
	  virtual int sum(int a, int b) ;

};

  class DrivedA : public Base
  {
	 int sum(int a, int b) override; // override int sum(int a, int b);
	 // double sum(int a, int b); // error!!
	  //double sum(int a); //  dose not override the int sum(int a, int b);
	  int sum(int a);

  };


  class Parent{
  public:
	  char c;
	  char b;

	  int a;
	  //virtual void say(){
		 // cout << "I am Parent";
	  //}
  };

  class Children :public Parent{
	  virtual void say(){
		  cout << "I am Children";
	  }
  };
  

void keyboardUpdate(GLFWwindow *window, GameObject *bindObject)
{
 
	//bool isFirstobj = true;

	float speed = 3.0f;

	mat4 inverseProj = inverse(projectionMatrix);
	mat4 inverseView = inverse(camera.getWorldToViewMatrix());
	mat4 transMatrix = inverseView * inverseProj;
 

	vec4 rightDirInworld = transMatrix* vec4(1, 0, 0, 0);
	normalize(rightDirInworld);
	vec3 ForwardDirInworld =- glm::cross(vec3(rightDirInworld), vec3((bindObject->getModelToWorldMatrix())[1]));  //vec4(0, 0, -1, 0);//transMatrix* vec4(0, 0, -1, 0);
	normalize(ForwardDirInworld);
	//vec4(1, 0, 0, 0); //(bindObject->getModelToWorldMatrix())[0];//
//	glm::vec3 right = glm::cross(viewDirection, Up);  //right vector

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		//bindObject->position += vec3(ForwardDirInworld )* deltaTime * speed;
		bindObject->m_velocity =speed*ForwardDirInworld + bindObject->m_velocity.y *vec3(0,1,0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));

	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		//bindObject->position -= vec3(ForwardDirInworld) * deltaTime * speed;
		bindObject->m_velocity = -speed*ForwardDirInworld + bindObject->m_velocity.y *vec3(0, 1, 0);

		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));

	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
	//	bindObject->position +=vec3( rightDirInworld) * deltaTime * speed;
		bindObject->m_velocity = speed * vec3(rightDirInworld) + bindObject->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));

  	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
	//	bindObject->position -= vec3(rightDirInworld) * deltaTime * speed;
		bindObject->m_velocity = -speed * vec3(rightDirInworld) + bindObject->m_velocity.y *vec3(0, 1, 0);

		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));

	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		bindObject->m_velocity = speed * (vec3(rightDirInworld) + ForwardDirInworld) *0.7f + bindObject->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		bindObject->m_velocity = speed * (-vec3(rightDirInworld) + ForwardDirInworld) *0.7f + bindObject->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		bindObject->m_velocity = speed * (vec3(rightDirInworld) - ForwardDirInworld) * 0.7f + bindObject->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		bindObject->m_velocity = speed * (-vec3(rightDirInworld) - ForwardDirInworld) *0.7f + bindObject->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));
	}



	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		bindObject->m_angle += (60 * deltaTime);
		bindObject->setModelMatrix(rotate(radians(bindObject->m_angle), vec3(0, 1, 0)), bindObject->translateMatrix);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
		bindObject->m_angle -= (60 * deltaTime);
		bindObject->setModelMatrix(rotate(radians(bindObject->m_angle), vec3(0, 1, 0)), bindObject->translateMatrix);
	}
	static bool isSpacePress = false;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isSpacePress){
		isSpacePress = true;
		bindObject->m_velocity += vec3(0, 10, 0);
		bindObject->isJump = true;
		cout << "fy" << bindObject->m_force.y << endl;

	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && isSpacePress){
		isSpacePress = false;
	 
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE  
		)
	{
		bindObject->m_velocity = vec3(0, bindObject->m_velocity.y, 0);

	}

//	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	//bindObject->m_velocity = vec3(0, 0, 0);
	// For the next frame, the "last time" will be "now"

}


int main(void)
{
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

 

 
	  float angle = 0;
 
 	  //(const char* objfile, const char* ddsfile, std::map<std::string, std::vector<GLuint>> &existList, vec3 _position, const char* vertexShader, const char* fragmentShader)
	  GameObject *box1 = new GameObject("Asset/crate.obj", "Asset/Crate_1.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", objList,
		  vec3(1,2.5,-3.0f), 0);	  
//	  box1.setModelMatrix(rotate(45.0f, vec3(0, 1, 0)), box1.translateMatrix);
	  box1->createBoundingBox();

	  GameObject *box2 = new GameObject("Asset/crate.obj", "Asset/Crate_1.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", objList,
		  vec3(5.0f, 1, -3.0f),0);
	  box2->createBoundingBox();
	  box2->rotationMatrix = rotate(radians(60.0f), vec3(0, 0, 1));
	  box2->m_mass = 100000;

	  GameObject *box3 = new GameObject("Asset/crate.obj", "Asset/Crate_1.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", objList,
		  vec3(3.0f, 1, -3.0f), 0);
	  box3->createBoundingBox();
	  box3->m_mass = 1;


	  GameObject gameobj2("Asset/skybox.obj", "Asset/skybox.dds", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", objList,
		  vec3(0, -1.0f, 0),0);
	  gameobj2.setModelMatrix(rotate(radians(-90.0f), vec3(1 ,0, 0)), gameobj2.translateMatrix);
 
	  GroundObject *gameobj3 = new GroundObject("Asset/ground.obj", "Asset/ground.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", objList,
		  vec3(0, -0.5f, 0));
	  gameobj3->createBoundingBox();


	  float terrainHeigh = 0.002f;

	  MousePicker *mousePicker = new MousePicker(camera, projectionMatrix, window);
	 // OcTreeNode::getRootInstance()->ProcessItem(&box2);
	  GameObject *bindObject = box1;
	  bool isTabPress = false;
	  // glfwGetTime is called only once, the first time this function is called
	  lt = glfwGetTime();

	  OcTreeNode::m_pendingInsertion.push(box1);
	  OcTreeNode::m_pendingInsertion.push(box2);
	  OcTreeNode::m_pendingInsertion.push(box3);
	  OcTreeNode::getRootInstance()->UpdateTree();

	  cout << "box1 owner " << endl;  box1->m_owner->m_region->PrintInfo();
	  cout << "bx21 owner " << endl;  box2->m_owner->m_region->PrintInfo();
 
 
 
 	do{

 
		// Compute time difference between current and last frame
		  currentTime = glfwGetTime();
		  deltaTime = float(currentTime - lt);


		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mousePicker->update(camera);
		vec3 test = mousePicker->getCurrentRay();
		//std::cout << "x " << test.x << "y " << test.y << "z " << test.z << std::endl;


		//catch mouse movement
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		camera.mouseUpdate(vec2(xpos, ypos));
		camera.keyboardUpdate(window);

		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && isTabPress == false){
			cout << "press" << endl;
			isTabPress = true;
		}
		if (isTabPress == true && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		{
			if (bindObject == box1){
				bindObject->m_velocity = vec3(0, 0, 0);

				bindObject = box2;
				cout << "box1->box2" << endl;
			}
			else if (bindObject == box2)
			{
				bindObject->m_velocity = vec3(0, 0, 0);

				bindObject = box1;
				cout << "box2->box1" << endl;
			}
			isTabPress = false;
		}

		keyboardUpdate(window, bindObject);

 

		box1->Update(deltaTime);
		box2->Update(deltaTime);
		box3->Update(deltaTime);

 		CollisionDetection::processCollisionRespond(OcTreeNode::getRootInstance()->GetIntersectionList(vector<GameObject*>())  , deltaTime);

		box1->Draw(camera.getWorldToViewMatrix());
		box2->Draw(camera.getWorldToViewMatrix());
		box3->Draw(camera.getWorldToViewMatrix());
		mat4 viewMatrixSky = camera.getWorldToViewMatrix();
		viewMatrixSky[3] = vec4(0, 0, 0, 1); 
		gameobj2.Draw(viewMatrixSky);
		gameobj3->Draw( camera.getWorldToViewMatrix());

		///////////////  draw bounding box ////////////////////////////
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		box1->m_obb->drawBox(box1->m_fullMatrix);
		box2->m_obb->drawBox(box2->m_fullMatrix);
		box3->m_obb->drawBox(box3->m_fullMatrix);
 		///////////////  draw bounding box ////////////////////////////

		//draw octree
		OcTreeNode::drawDebugBox(OcTreeNode::getRootInstance(), projectionMatrix * camera.getWorldToViewMatrix());

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
