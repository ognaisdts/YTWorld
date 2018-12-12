#include "RenderManager.h"

vector<GameObject*> RenderManager::registedList;


 void RenderManager::RegisterGameObject(GameObject* gameobject)
{
	 registedList.push_back(gameobject);
}
