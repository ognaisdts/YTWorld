#include "TestScene.h"


TestScene::TestScene(GLFWwindow *_window, Camera *_camera)
{
	m_window = _window;
	m_camera = _camera;
}

GameObject *box1, *box2, *box3, *groundObj, *skyBox;
GameObject *bindObject;

void TestScene::BeginScene() 
{
	box1 = new GameObject(vec3(1, 2.5, -3.0f), 0);
	box1->AddRenderComponent("Asset/crate.obj", "Asset/Crate_1.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	box1->AddRigidBody();
	box1->CreateBoundingBox();

	box2 = new GameObject(vec3(5.0f, 1, -3.0f), 0);
	box2->AddRenderComponent("Asset/crate.obj", "Asset/Crate_1.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	box2->AddRigidBody(100000);
	box2->CreateBoundingBox();
	box2->rotationMatrix = rotate(radians(60.0f), vec3(0, 0, 1));


	box3 = new GameObject(vec3(3.0f, 1, -3.0f), 0);
	box3->AddRenderComponent("Asset/crate.obj", "Asset/Crate_1.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	box3->AddRigidBody();
	box3->CreateBoundingBox();


	skyBox = new GameObject(vec3(0, -1.0f, 0), 0);
	skyBox->setModelMatrix(rotate(radians(-90.0f), vec3(1, 0, 0)), skyBox->translateMatrix);
	skyBox->AddRenderComponent("Asset/skybox.obj", "Asset/skybox.dds", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader", true);


	groundObj = new GroundObject(vec3(0, 0, 0));
	groundObj->AddRenderComponent("Asset/ground.obj", "Asset/ground.DDS", "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	groundObj->CreateBoundingBox();

	OcTreeNode::m_pendingInsertion.push(box1);
	OcTreeNode::m_pendingInsertion.push(box2);
	OcTreeNode::m_pendingInsertion.push(box3);
	OcTreeNode::getRootInstance()->UpdateTree();
}

bool isTabPress = false;

void TestScene::UpdateScene(float deltaTime)
{
	if (isTabPress == true && glfwGetKey(m_window, GLFW_KEY_TAB) == GLFW_RELEASE)
	{
		if (bindObject == box1 && bindObject->m_rigid != nullptr) {
			bindObject->m_rigid->m_velocity = vec3(0, 0, 0);

			bindObject = box2;
			cout << "box1->box2" << endl;
		}
		else if (bindObject == box2 && bindObject->m_rigid != nullptr)
		{
			bindObject->m_rigid->m_velocity = vec3(0, 0, 0);

			bindObject = box1;
			cout << "box2->box1" << endl;
		}
		isTabPress = false;
	}

	keyboardUpdate(m_window, bindObject, deltaTime);

	box1->Update(deltaTime);
	box2->Update(deltaTime);
	box3->Update(deltaTime);


	///////////////  draw bounding box ////////////////////////////
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	box1->m_obb->drawBox(box1->projectionMatrix * m_camera->getWorldToViewMatrix() * box1->getModelToWorldMatrix());
	box2->m_obb->drawBox(box2->projectionMatrix * m_camera->getWorldToViewMatrix() * box2->getModelToWorldMatrix());
	box3->m_obb->drawBox(box3->projectionMatrix * m_camera->getWorldToViewMatrix() * box3->getModelToWorldMatrix());
	///////////////  draw bounding box ////////////////////////////

}

void TestScene::EndScene()
{

}
TestScene::~TestScene()
{
}


void TestScene::keyboardUpdate(GLFWwindow *window, GameObject *bindObject, float deltaTime)
{

	if (bindObject == nullptr)
		bindObject = box1;


	float speed = 3.0f;

	mat4 inverseProj = inverse(bindObject->projectionMatrix);
	mat4 inverseView = inverse(m_camera->getWorldToViewMatrix());
	mat4 transMatrix = inverseView * inverseProj;


	vec4 rightDirInworld = transMatrix* vec4(1, 0, 0, 0);
	normalize(rightDirInworld);
	vec3 ForwardDirInworld = -glm::cross(vec3(rightDirInworld), vec3((bindObject->getModelToWorldMatrix())[1]));  //vec4(0, 0, -1, 0);//transMatrix* vec4(0, 0, -1, 0);
	normalize(ForwardDirInworld);
	//vec4(1, 0, 0, 0); //(bindObject->getModelToWorldMatrix())[0];//
	//	glm::vec3 right = glm::cross(viewDirection, Up);  //right vector

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//bindObject->position += vec3(ForwardDirInworld )* deltaTime * speed;
		bindObject->m_rigid->m_velocity = speed*ForwardDirInworld + bindObject->m_rigid->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));

	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//bindObject->position -= vec3(ForwardDirInworld) * deltaTime * speed;
		bindObject->m_rigid->m_velocity = -speed*ForwardDirInworld + bindObject->m_rigid->m_velocity.y *vec3(0, 1, 0);

		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));

	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//	bindObject->position +=vec3( rightDirInworld) * deltaTime * speed;
		bindObject->m_rigid->m_velocity = speed * vec3(rightDirInworld) + bindObject->m_rigid->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));

	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//	bindObject->position -= vec3(rightDirInworld) * deltaTime * speed;
		bindObject->m_rigid->m_velocity = -speed * vec3(rightDirInworld) + bindObject->m_rigid->m_velocity.y *vec3(0, 1, 0);

		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));

	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		bindObject->m_rigid->m_velocity = speed * (vec3(rightDirInworld) + ForwardDirInworld) *0.7f + bindObject->m_rigid->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		bindObject->m_rigid->m_velocity = speed * (-vec3(rightDirInworld) + ForwardDirInworld) *0.7f + bindObject->m_rigid->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		bindObject->m_rigid->m_velocity = speed * (vec3(rightDirInworld) - ForwardDirInworld) * 0.7f + bindObject->m_rigid->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		bindObject->m_rigid->m_velocity = speed * (-vec3(rightDirInworld) - ForwardDirInworld) *0.7f + bindObject->m_rigid->m_velocity.y *vec3(0, 1, 0);
		bindObject->setModelMatrix(bindObject->rotationMatrix, translate((bindObject->position)));
	}



	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		bindObject->m_angle += (60 * deltaTime);
		bindObject->setModelMatrix(rotate(radians(bindObject->m_angle), vec3(0, 1, 0)), bindObject->translateMatrix);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		bindObject->m_angle -= (60 * deltaTime);
		bindObject->setModelMatrix(rotate(radians(bindObject->m_angle), vec3(0, 1, 0)), bindObject->translateMatrix);
	}
	static bool isSpacePress = false;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isSpacePress) {
		isSpacePress = true;
		bindObject->m_rigid->m_velocity += vec3(0, 10, 0);
		bindObject->isJump = true;
		cout << "fy" << bindObject->m_rigid->m_force.y << endl;

	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && isSpacePress) {
		isSpacePress = false;

	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE
		)
	{
		bindObject->m_rigid->m_velocity = vec3(0, bindObject->m_rigid->m_velocity.y, 0);

	}

	//	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	//bindObject->m_velocity = vec3(0, 0, 0);
	// For the next frame, the "last time" will be "now"

}
