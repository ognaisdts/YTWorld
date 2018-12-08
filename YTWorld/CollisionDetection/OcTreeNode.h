#pragma once
#include "..\GameObject.h"
#include <vector>
#include <set>
#include <queue>          // std::queue

using namespace glm;
using namespace std;

class GameObject;

struct IntersectInfo;

typedef unsigned char BYTE;



class OcTreeNode
{
public:

	//singleton
	static OcTreeNode *octreeRoot;
	static OcTreeNode* getRootInstance()
	{
		if (octreeRoot == nullptr)
		{
			// attend the root thar can cover whole world in this test octtree
			octreeRoot = new OcTreeNode(nullptr, vec3(0, 0, 0), 32.0, vector<GameObject*>());
 
		}
		else
		{
			return octreeRoot;
		}
	}

	OcTreeNode(OcTreeNode *_parent, vec3 _nodePos, float _node_halfLength, vector<GameObject*> _potentialObj)
	{
		this->parent = _parent;
		halfDimentionLength = _node_halfLength;
		nodePos = _nodePos;



		m_objects = _potentialObj;
		for (int i = 0; i < 8; ++i)
		{
			childrenNode[i] = nullptr;
		}
 

		m_region = new BoundingBox(nodePos + vec3(-_node_halfLength, -_node_halfLength, -_node_halfLength), nodePos + vec3(_node_halfLength, _node_halfLength, _node_halfLength), 1.0f);
 
	}


	////// method ///////
	void UpdateTree();
	void BuildTree();
	void FindEnclosingBox();
	void FindingEnclosingCube();
	void InsertObjects(GameObject* object);
	bool IsObjInsideBox(GameObject* obj, BoundingBox * region);
	OcTreeNode * CreateNode(BoundingBox* region, vector<GameObject*> objList);
	void InsertInExistNode(GameObject *obj);

//	void Update(float deltaTime);

	~OcTreeNode(); 

	// member
//	static int maxObjectLimit;//= 1;
	float halfDimentionLength; // the length from the center of node Node to one of its wall
	/// The minumum size for enclosing region is a "1x1x1 cube"
 	const int MIN_SIZE = 1;

	vec3 nodePos; // the center of this node

	OcTreeNode *parent;

	// what obj s are contained in THIS PARTICULAR octree node
	vector<GameObject*> m_objects;

	OcTreeNode *childrenNode[8]; // 8 children

	BoundingBox *m_region; // draw this node

	// extra info 

	/// This is a bitmask indicating which child nodes are actively being used.
	/// It adds slightly more complexity, but is faster for performance since there is only one comparison instead of 8.
	BYTE m_activeNodes = 0;

 	/// These are items which we're waiting to insert into the data structure. 
	/// We want to accrue as many objects in here as possible before we inject them into the tree. This is slightly more cache friendly.
	static std::queue<GameObject*> m_pendingInsertion;  

	/// <summary>
	/// this is how many frames we'll wait before deleting an empty tree branch. Note that this is not a constant. The maximum lifespan doubles
	/// every time a node is reused, until it hits a hard coded constant of 64
	/// </summary>
	int m_maxLifespan = 8;          //
	int m_curLife = -1;             //this is a countdown time showing how much time we have left to live

	//////////  new idea
	bool OcTreeNode::CanKill( );
	static void KillNode(OcTreeNode *target, GameObject *obj);

	static bool m_treeReady;        //the tree has a few objects which need to be inserted before it is complete
	static bool m_treeBuilt;    //there is no pre-existing tree yet.

	 
	static void drawDebugBox(OcTreeNode *root, mat4 transformMatrix)
	{
		if (root == nullptr) return;
	
		for each (OcTreeNode *var in root->childrenNode)
		{
			if (var != nullptr)
			{
				drawDebugBox(var, transformMatrix);
				
			}
		}
		root->m_region->drawBox(transformMatrix);
	}


	  vector<IntersectInfo*> GetIntersectionList(vector<GameObject *> &parentObjects);

};


// Represents a single point of contact during a collision
struct Contact
{
	vec3 position;
	vec3 normal;
	float penetration;
};

struct Manifold
{
	Contact contacts[8]; // box (8 vertice ) in BOX...
	unsigned contactCount;
	GameObject *objA;
	GameObject *objB;
};


struct IntersectInfo
{
	IntersectInfo(GameObject *_intersectedObject = nullptr)
	{
		//intersectedObject = nullptr;
		//intersectedOtherObject = nullptr;
		manifold.objA = nullptr;
		manifold.objB = nullptr;
		manifold.contactCount = 0;
		OcTreeNode * treeNode = nullptr;
		hasHit = (manifold.objA != nullptr) ? true : false;
	}

	//GameObject *intersectedObject;
	//GameObject *intersectedOtherObject;
	//vec3 intersectPoint;
	//vec3 normal;
	Manifold manifold;
	OcTreeNode * treeNode;
	bool hasHit;
};
