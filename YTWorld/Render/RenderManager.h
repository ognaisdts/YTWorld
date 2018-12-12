#pragma once
#include "RenderComponent.h"
#include <vector>
#include "..\GameObject.h"
#include "glm\glm.hpp"
#include "..\Camera.h"

using namespace std;

class RenderManager
{
	public:
	static vector<GameObject*> registedList;
	Camera *m_camera;
	RenderManager(Camera* _camera)
	{
		m_camera = _camera;
	}


	static RenderComponent* CreateRenderComponent(const char* objFilePath, const char* ddsFilePath, const char* vertexShader, const char* fragmentShader)
	{
		return new RenderComponent(objFilePath, ddsFilePath, vertexShader, fragmentShader);
	}

	static void RegisterGameObject(GameObject* gameobject);
	void Render()
	{
		for each (GameObject* obj in registedList)
		{
			glm::mat4 worldToViewMatrix = m_camera->getWorldToViewMatrix(); // from camera
			if (obj->m_renderComponent->m_isSky)
			{
				worldToViewMatrix[3] = vec4(0, 0, 0, 1);
			}
			glm::mat4	fullMatrix = obj->projectionMatrix*worldToViewMatrix* obj->getModelToWorldMatrix();
			obj->m_renderComponent->Render(fullMatrix);
		}
	}

	//TODO: Implement Remove RenderComponent
};