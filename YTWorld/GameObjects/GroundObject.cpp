#include "GroundObject.h"


//GroundObject::GroundObject(const char* objfile,
//	const char* ddsfile,
//	const char* vertexShader,
//	const char* fragmentShader,
//	std::map<std::string, Mesh> &existList,
//	vec3 _position,
//	float angle) : GameObject( objfile,
//	  ddsfile,
//	  vertexShader,
//	  fragmentShader,
//	  existList,
//	  _position,
//	  angle)
//{
//}


GroundObject::~GroundObject()
{
}

void GroundObject::Update(float deltaTime)
{
	/*
	refreshOwner();


	for each (OcTreeNode* owner in ownerNodes)
	{
		if (owner->parent != nullptr)
		{
			//	cout << "has parent!" << endl;
			for (int i = 0; i < 8; ++i)
			{
				if (owner->parent->childrenNode[i] != nullptr && owner->parent->childrenNode[i]->containedObjects.size() > 0)
				{
					for each (GameObject *objInContain in  owner->parent->childrenNode[i]->containedObjects)
					{
						if (!CollisionDetection::SATcheck(*this, *objInContain) && this != objInContain)
						{
							//cout << "collision!!" << " " << CollisionDetection::collicount++ << endl;
							objInContain->m_force = vec3(0, 0, 0); //+= vec3(0, 10, 0);
							//objInContain->m_velocity.y = 0;


							objInContain->position = objInContain->m_prevPos + vec3(0,0.001f,0);
						 //
						}
					}
					//detect collision

				}
			}
		}
	}
	*/
}