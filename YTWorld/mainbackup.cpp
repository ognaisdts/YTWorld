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
//#include <shader.hpp>
#include "Primitives\Vertex.h"
#include "Primitives\ShapeGenerator.h"
#include "Camera.h"
#include "CommonFunction\ObjLoader.h"
#include "CommonFunction\DDSLoader.h"
#include "GameObject.h"
#include "MousePicker.h"
using namespace glm;
using namespace std;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint numIndices;
Camera camera;


GLuint g_vertexbuffer;
GLuint g_vertexOffest = 0;
GLuint g_uvbuffer;
GLuint g_uvOffest = 0;
const uint MAX_NUM_VERTEX = 1024;
const uint MAX_VERTEX_BUFFER_SIZE = MAX_NUM_VERTEX   * sizeof(float) * 3;  //1024 vertex
const uint MAX_UV_BUFFER_SIZE = MAX_NUM_VERTEX* 2   * sizeof(float) ;  //1024 vertex
GLuint vertexbuffercount = 0;
GLuint uvbuffercount = 0;

GLuint objVertexArrayID;
//GLuint vertexPosition_modelspaceID;
//GLuint vertexUVID;
//GLuint TextureID;

GLuint buffers[2];

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals; // Won't be used at the moment.

std::map<string, Mesh>  objList; //obj name, vertex array ID,  numVertice
mat4 projectionMatrix = glm::perspective(glm::radians(67.0f), 640.f / 480.0f, 0.3f, 1000.0f);
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()){
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

std::string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "file failed to load...." << fileName;
		exit(1);
	}

	return string(std::istreambuf_iterator<char>(meInput), std::istreambuf_iterator<char>());
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	adapter[0] = readShaderCode("SimpleVertexShader.vertexshader" ).c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = readShaderCode("SimpleFragmentShader.fragmentshader").c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
 }

void sendDataToOpenGL()
{
 ///// tri
	/*
	ShapeData tri = ShapeGenerator::makeTriangle();
	GLuint vertexbufferID;
	glGenBuffers(1, &vertexbufferID);// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(myTri), myTri, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, tri.vertexBufferSize(), tri.vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1 , 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float)*3));

	GLshort indices[] = { 0, 1, 2 };
	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri.indexBufferSize(), tri.indices, GL_STATIC_DRAW);
	tri.cleanup();
	*/

	ShapeData shape = ShapeGenerator::makeCube();
	GLuint vertexbufferID;
	glGenBuffers(1, &vertexbufferID);// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
 	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.vertices, GL_STATIC_DRAW);

	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexColorID = glGetAttribLocation(programID, "vertexColor");

	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(vertexColorID);
	glVertexAttribPointer(vertexColorID, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));

 
	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);
	numIndices = shape.numIndices;
	shape.cleanup();
}

 
void sendObjToGL(const char *objFilePath,  const char * ddsFilePath, string objName)
{
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler"); 	  // Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");

	if (objList.find(objName) != objList.end()) return; // obj has already rigister

	bool res = loadOBJ(objFilePath, vertices, uvs, normals);

	// Load the texture
	GLuint	Texture = loadDDS(ddsFilePath);//("Asset/Crate_1.DDS");


	glGenVertexArrays(1, &objVertexArrayID);
	glBindVertexArray(objVertexArrayID);
	 
	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind buffer for colors and copy data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexUVID);
	glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, 0);
 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);


 

}
void paintGL()
{

	// transformation
	/*
	mat4 rotationMatrix = rotate(mat4(), 54.0f, vec3(1.0f, 0, 0));
	mat4 translateMatrix = translate(mat4(), vec3(0.0f, 0.0f, -3.0f));
	mat4 projectionMatrix = perspective(60.0f, 680.0f / 480.0f, 0.1f, 10.0f);
	mat4 fullTransformMatrix = projectionMatrix * translateMatrix * rotationMatrix;
	*/
	/*
	mat4 projectionMatrix = perspective(60.0f, 680.0f / 480.0f, 0.1f, 10.0f);
	mat4 projectionTranslateMatrix = translate(projectionMatrix, vec3(0.0f, 0.0f, -3.0f));
	mat4 fullTransformMatrix = rotate(projectionTranslateMatrix, 54.0f, vec3(1.0f, 0, 0));
	*/

	GLint fullTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "fullTransformMatrix");
	
 
	//cube 1
	mat4 fullTransformMatrix;
	mat4 projectionMatrix = glm::perspective(  glm::radians(60.0f) , 640.f / 480.0f, 0.1f, 100.0f);
	mat4 translateMatrix =glm:: translate( vec3(-1.0f, 0.0f, -3.0f));
	mat4 rotationMatrix = glm::rotate(36.0f, vec3(1.0f, 0.0f, 0.0f));   // include gtx\transform

	fullTransformMatrix = projectionMatrix * camera.getWorldToViewMatrix() * translateMatrix * rotationMatrix;

	//pass data
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	// glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle 
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	//cube 2
	translateMatrix = translate(vec3(1.0f, 0.0f, -3.75f));
	rotationMatrix = rotate(45.0f, vec3(0.0f, 1.0f, 0.0f));   // include gtx\transform
	fullTransformMatrix = projectionMatrix * camera.getWorldToViewMatrix() * translateMatrix * rotationMatrix;

	//pass data
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

}

void keyboardUpdate(GLFWwindow *window , GameObject &bindObject)
{
	float speed = 10.0f;
	// glfwGetTime is called only once, the first time this function is called
	static double lt = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lt);
	mat4 inverseProj = inverse(projectionMatrix);
	mat4 inverseView = inverse(camera.getWorldToViewMatrix());
	mat4 transMatrix = inverseView * inverseProj;
 

	vec4 rightDirInworld = transMatrix* vec4(1, 0, 0, 0);
	vec3 ForwardDirInworld =- glm::cross(vec3(rightDirInworld), vec3((bindObject.getModelToWorldMatrix())[1]));  //vec4(0, 0, -1, 0);//transMatrix* vec4(0, 0, -1, 0);
	//vec4(1, 0, 0, 0); //(bindObject.getModelToWorldMatrix())[0];//
//	glm::vec3 right = glm::cross(viewDirection, Up);  //right vector

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		bindObject.position += vec3(ForwardDirInworld )* deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		bindObject.position -= vec3(ForwardDirInworld) * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		bindObject.position +=vec3( rightDirInworld) * deltaTime * speed;
  	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		bindObject.position -= vec3(rightDirInworld) * deltaTime * speed;
	}
	bindObject.setModelMatrix(bindObject.rotationMatrix, translate((bindObject.position)));

	// For the next frame, the "last time" will be "now"
	lt = currentTime;
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

 
	// Create and compile our GLSL program from the shaders
	 // programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

 
	  float angle = 0;
 
 	  //(const char* objfile, const char* ddsfile, std::map<std::string, std::vector<GLuint>> &existList, vec3 _position, const char* vertexShader, const char* fragmentShader)
	  GameObject box1("Asset/crate.obj", "Asset/Crate_1.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", objList,
		  vec3(0,0,-3.0f));	  
	  box1.setModelMatrix(rotate(45.0f, vec3(0, 1, 0)), box1.translateMatrix);
	  box1.CreateBoundingBox();

	  GameObject box2("Asset/crate.obj", "Asset/Crate_1.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", objList,
		  vec3(3.0f, 0, -3.0f));
	  box2.CreateBoundingBox();


	  GameObject gameobj2("Asset/skybox.obj", "Asset/skybox.dds", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", objList,
		  vec3(0, -1.0f, 0));
	  gameobj2.setModelMatrix(rotate(radians(-90.0f), vec3(1 ,0, 0)), gameobj2.translateMatrix);
	//  gameobj2.CreateBoundingBox();
	  GameObject gameobj3("Asset/ground.obj", "Asset/ground.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", objList,
		  vec3(0, -0.5f, 0));
	  gameobj3.CreateBoundingBox();

	  MousePicker *mousePicker = new MousePicker(camera, projectionMatrix, window);


	do{

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

		keyboardUpdate(window, box1);


		//box1.setModelMatrix(rotate(angle, vec3(0, 1, 0)), box1.translateMatrix);
	//	angle += 0.001f;

		box1.Draw(camera.getWorldToViewMatrix());
		box2.Draw(camera.getWorldToViewMatrix());
		mat4 viewMatrixSky = camera.getWorldToViewMatrix();
		viewMatrixSky[3] = vec4(0, 0, 0, 1);
 
		gameobj2.Draw(viewMatrixSky);
		gameobj3.Draw( camera.getWorldToViewMatrix());

		///////////////  draw bounding box ////////////////////////////
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		box1.m_obb->drawBox(box1.m_fullMatrix);
		box2.m_obb->drawBox(box2.m_fullMatrix);

	//	gameobj3.m_obb->drawBox(gameobj3.m_fullMatrix);
		///////////////  draw bounding box ////////////////////////////


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);


	// Cleanup VBO
//	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
