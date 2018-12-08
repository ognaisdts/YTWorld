#include "OcTreeNode.h"

 
OcTreeNode* OcTreeNode::octreeRoot;
std::queue<GameObject*> OcTreeNode::m_pendingInsertion;
bool OcTreeNode::m_treeReady = false;       //the tree has a few objects which need to be inserted before it is complete
bool OcTreeNode::m_treeBuilt = false;       //there is no pre-existing tree yet.



void OcTreeNode::UpdateTree() // might root only
{
	if (!m_treeBuilt)
	{
		while (m_pendingInsertion.size() != 0)
		{
			m_pendingInsertion.front()->m_owner = this;
			m_objects.push_back(m_pendingInsertion.front());
			m_pendingInsertion.pop();
		}
		this->BuildTree();  
		//once the tree build success  this->m_treeBuilt will be true then rurn the next scope
		//because the tree has already built.  if  m_pendingInsertion increase in run-time...  call the InsertObjects() method 

	}
	else
	{
		while (m_pendingInsertion.size() != 0)
		{
			GameObject *tempObj = m_pendingInsertion.front();
			m_pendingInsertion.pop();
			this->InsertObjects(tempObj);
		}
	}

	this->m_treeReady = true;

}

void OcTreeNode::BuildTree()  
{
	//terminate the rescursion
	if (m_objects.size() <= 1) return;
	
	vec3 fullDimension = m_region->m_max - m_region->m_min;
	if (fullDimension == vec3(0, 0, 0))
	{
		FindingEnclosingCube();// update the m_region
		fullDimension = m_region->m_max - m_region->m_min;  
	}

	//Check to see if the dimensions of the box are greater than the minimum dimensions
	if (fullDimension.x <= MIN_SIZE && fullDimension.y <= MIN_SIZE && fullDimension.z <= MIN_SIZE)
	{
		// the current node region si less than 1x1x1 cube,   do nothing and return
		return;
	}

	vec3 halfDimension = fullDimension / 2.0f;
	 
	// subdivided m_region in 8 sub_region
	BoundingBox* octant[8];
	// up 4
	octant[0]=(new BoundingBox((nodePos + vec3(0, 0, -halfDimension.z)), (nodePos + vec3(halfDimension.x, halfDimension.y, 0)), 1.0f)); // min (0,0,-1) max(1,1,0)
	octant[1]=(new BoundingBox((nodePos), (m_region->m_max), 1.0f)); // min (0,0,0) max(1,1,1)
	octant[2]=(new BoundingBox((nodePos + vec3(-halfDimension.x, 0, 0)), (nodePos + vec3(0, halfDimension.y, halfDimension.z)), 1.0f)); // min (-1,0,0) max(0,1,1)
	octant[3]=(new BoundingBox((nodePos + vec3(-halfDimension.x, 0, -halfDimension.z)), (nodePos + vec3(0, halfDimension.y, 0)), 1.0f)); // min (-1,0,-1) max(0,1,0)
	//bottom 
	octant[4]=(new BoundingBox((nodePos + vec3(0, -halfDimension.y, -halfDimension.z)), (nodePos + vec3(halfDimension.x, 0, 0)), 1.0f));  // min (0,-1,-1) max(1,0,0)
	octant[5]=(new BoundingBox((nodePos + vec3(0, -halfDimension.y, 0)), (nodePos + vec3(halfDimension.x, 0, halfDimension.z)), 1.0f));  // min (0,-1,0) max(1,0,1)
	octant[6]=(new BoundingBox((nodePos + vec3(-halfDimension.x, -halfDimension.y, 0)), (nodePos + vec3(0, 0, halfDimension.z)), 1.0f)); // min (-1,-1,0) max(0,0,1)
	octant[7]=(new BoundingBox((m_region->m_min), (nodePos), 1.0f));  // min (-1,-1,-1) max(0,0,0)

	//This will contain all of our objects which fit within each respective octant.
	 vector<GameObject*>  objContainer[8]; // for each potential octant
	vector<GameObject*> delist; // suppose move down to hildren and remove in this->m_objects;

	for each(GameObject* obj in this->m_objects)
	{
		if (obj->m_obb->m_min != obj->m_obb->m_max)
		{
			// check obj in each sub region
			for (int i = 0; i < 8; i++)
			{
				if (this->IsObjInsideBox(obj, octant[i]))
				{
					objContainer[i].push_back(obj);
					delist.push_back(obj);
				}
			}
		}
		//else if (bounding sphere)   //  keep for sphere....  later !?
	}

	// remove the objects which in delist form m_objects
	for each(GameObject *removeTarget in delist)
	{
		auto element = find(m_objects.begin(), m_objects.end(), removeTarget);
		if (element != m_objects.end())
		{
			(*element)->m_owner = nullptr;
			m_objects.erase(element);
		}
	}

	//create new OcTreeNode for each subdivided
	for (int i = 0; i < 8; i++)
	{
		if (objContainer[i].size() > 0)
		{
			childrenNode[i] = CreateNode( octant[i], objContainer[i]  );
			m_activeNodes |= (BYTE)(1 << i);
			childrenNode[i]->BuildTree();
		}
	}
	m_treeBuilt = true;
	m_treeReady = true;

	for (int i = 0; i < 8; i++)
	{
		delete octant[i];
		octant[i] = nullptr;
	}
}					
OcTreeNode * OcTreeNode::CreateNode(BoundingBox* region, vector<GameObject*> objList)
{
	if (objList.size() == 0)  return nullptr;

	OcTreeNode *res = new OcTreeNode(this, region->m_center, region->m_size.x / 2.0f, objList);
	for each(GameObject* obj in objList)
	{
		obj->m_owner = res;
	}
	
	return res;
}


bool OcTreeNode::IsObjInsideBox(GameObject* obj, BoundingBox * region)
{
	if (region == nullptr) return false;

	//find obj's obb max, min in world space
	mat3 modelToworld =mat3( obj->getModelToWorldMatrix());
	//calcualte 8 point 
	// box size would not change..
	vec3 boxSize = obj->m_obb->m_size;  //m_max - m_min
	vec3 halfSize = boxSize / 2.0f;
	vec3 boxCenter = obj->position;
	vec3 points[8] =
	{
		boxCenter + modelToworld* halfSize*vec3(1, 1, 1),
		boxCenter + modelToworld* halfSize*vec3(-1 ,1,1 ),
		boxCenter + modelToworld* halfSize*vec3(-1, 1,-1),
		boxCenter + modelToworld* halfSize*vec3(1,1,-1),
		boxCenter + modelToworld* halfSize*vec3(1 ,-1, 1),
		boxCenter + modelToworld* halfSize*vec3(-1, -1,1 ),
		boxCenter + modelToworld* halfSize*vec3(-1, -1,-1 ),
		boxCenter + modelToworld* halfSize*vec3(1, -1, -1)
	};

	vec3 pos = (region->m_max + region->m_min) / 2.0f;
	float halfDimentionLength = (region->m_max.x - region->m_min.x) / 2.0f;
	for (int i = 0; i < 8; ++i)
	{
		vec3 objPos = points[i];
		if (objPos.x > pos.x + halfDimentionLength || objPos.x < pos.x - halfDimentionLength)
			return false;
		if (objPos.y > pos.y + halfDimentionLength || objPos.y < pos.y - halfDimentionLength)
			return false;
		if (objPos.z > pos.z + halfDimentionLength || objPos.z < pos.z - halfDimentionLength)
			return false;
	}

	return true;
 
	/*
	////////////////////////////////////////////////workable///////////////////////////////////////////////////////
	vec3 objPos = obj->position;
	vec3 pos = (region->m_max + region->m_min) / 2.0f;
	float halfDimentionLength = (region->m_max.x - region->m_min.x) / 2.0f;
	if (objPos.x > pos.x + halfDimentionLength || objPos.x < pos.x - halfDimentionLength)
		return false;
	if (objPos.y > pos.y + halfDimentionLength || objPos.y < pos.y - halfDimentionLength)
		return false;
	if (objPos.z > pos.z + halfDimentionLength || objPos.z < pos.z - halfDimentionLength)
		return false;
	return true;
	////////////////////////////////////////////////workable///////////////////////////////////////////////////////
	*/


}
 
void OcTreeNode::InsertInExistNode(GameObject *object)
{
	vec3 dimensions = m_region->m_max - m_region->m_min;

	vec3 halfDimension = dimensions / 2.0f;

	// subdivided m_region in 8 sub_region
	BoundingBox* octant[8];
	// up 4
	octant[0] = (childrenNode[0] != nullptr) ? childrenNode[0]->m_region : (new BoundingBox((nodePos + vec3(0, 0, -halfDimension.z)), (nodePos + vec3(halfDimension.x, halfDimension.y, 0)), 1.0f)); // min (0,0,-1) max(1,1,0)
	octant[1] = (childrenNode[1] != nullptr) ? childrenNode[1]->m_region : (new BoundingBox((nodePos), (m_region->m_max), 1.0f)); // min (0,0,0) max(1,1,1)
	octant[2] = (childrenNode[2] != nullptr) ? childrenNode[2]->m_region : (new BoundingBox((nodePos + vec3(-halfDimension.x, 0, 0)), (nodePos + vec3(0, halfDimension.y, halfDimension.z)), 1.0f)); // min (-1,0,0) max(0,1,1)
	octant[3] = (childrenNode[3] != nullptr) ? childrenNode[3]->m_region : (new BoundingBox((nodePos + vec3(-halfDimension.x, 0, -halfDimension.z)), (nodePos + vec3(0, halfDimension.y, 0)), 1.0f)); // min (-1,0,-1) max(0,1,0)

	octant[4] = (childrenNode[4] != nullptr) ? childrenNode[4]->m_region : (new BoundingBox((nodePos + vec3(0, -halfDimension.y, -halfDimension.z)), (nodePos + vec3(halfDimension.x, 0, 0)), 1.0f));  // min (0,-1,-1) max(1,0,0)
	octant[5] = (childrenNode[5] != nullptr) ? childrenNode[5]->m_region : (new BoundingBox((nodePos + vec3(0, -halfDimension.y, 0)), (nodePos + vec3(halfDimension.x, 0, halfDimension.z)), 1.0f));  // min (0,-1,0) max(1,0,1)
	octant[6] = (childrenNode[6] != nullptr) ? childrenNode[6]->m_region : (new BoundingBox((nodePos + vec3(-halfDimension.x, -halfDimension.y, 0)), (nodePos + vec3(0, 0, halfDimension.z)), 1.0f)); // min (-1,-1,0) max(0,0,1)
	octant[7] = (childrenNode[7] != nullptr) ? childrenNode[7]->m_region : (new BoundingBox((m_region->m_min), (nodePos), 1.0f));  // min (-1,-1,-1) max(0,0,0)
	if (object->m_obb->m_max != object->m_obb->m_min && IsObjInsideBox(object, m_region))
	{
		bool found = false;
		//we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
		for (int a = 0; a<8; a++)
		{
			//is the object fully contained within a quadrant?
			if (IsObjInsideBox(object, octant[a]))//childOctant[a].Contains(Item.BoundingBox) == ContainmentType.Contains)
			{
				if (childrenNode[a] != nullptr)
					childrenNode[a]->InsertInExistNode(object);   //Add the item into that tree and let the child tree figure out what to do with it
				else
				{
					childrenNode[a] = CreateNode(octant[a], vector<GameObject*>(1, object));//CreateNode(childOctant[a], Item);   //create a new tree node with the item
					m_activeNodes |= (BYTE)(1 << a);
				}
				found = true;
			}
		}
		if (!found)
		{
			object->m_owner = this;

			m_objects.push_back(object);
		}

	}
	for (int i = 0; i < 8; i++)
	{
		if (childrenNode[i] == nullptr)
		{
			delete octant[i];
			octant[i] = nullptr;
		}
	}
}

void OcTreeNode::InsertObjects(GameObject* object)
{
	/*make sure we're not inserting an object any deeper into the tree than we have to.
	-if the current node is an empty leaf node, just insert and leave it.*/
	if (m_objects.size() <= 1 && m_activeNodes == 0)
	{
		object->m_owner = this;
		m_objects.push_back(object);
		return;
	}
	vec3 dimensions = m_region->m_max - m_region->m_min;
	//Check to see if the dimensions of the box are greater than the minimum dimensions
	if (dimensions.x <= MIN_SIZE && dimensions.y <= MIN_SIZE && dimensions.z <= MIN_SIZE)
	{
		object->m_owner = this;
		m_objects.push_back(object);
		return;
	}

	vec3 halfDimension = dimensions / 2.0f;

	// subdivided m_region in 8 sub_region
	BoundingBox* octant[8];
	// up 4
	octant[0] = (childrenNode[0] != nullptr) ? childrenNode[0]->m_region : (new BoundingBox((nodePos + vec3(0, 0, -halfDimension.z)), (nodePos + vec3(halfDimension.x, halfDimension.y, 0)), 1.0f)); // min (0,0,-1) max(1,1,0)
	octant[1] = (childrenNode[1] != nullptr) ? childrenNode[1]->m_region : (new BoundingBox((nodePos), (m_region->m_max), 1.0f)); // min (0,0,0) max(1,1,1)
	octant[2] = (childrenNode[2] != nullptr) ? childrenNode[2]->m_region : (new BoundingBox((nodePos + vec3(-halfDimension.x, 0, 0)), (nodePos + vec3(0, halfDimension.y, halfDimension.z)), 1.0f)); // min (-1,0,0) max(0,1,1)
	octant[3] = (childrenNode[3] != nullptr) ? childrenNode[3]->m_region : (new BoundingBox((nodePos + vec3(-halfDimension.x, 0, -halfDimension.z)), (nodePos + vec3(0, halfDimension.y, 0)), 1.0f)); // min (-1,0,-1) max(0,1,0)

	octant[4] = (childrenNode[4] != nullptr) ? childrenNode[4]->m_region : (new BoundingBox((nodePos + vec3(0, -halfDimension.y, -halfDimension.z)), (nodePos + vec3(halfDimension.x, 0, 0)), 1.0f));  // min (0,-1,-1) max(1,0,0)
	octant[5] = (childrenNode[5] != nullptr) ? childrenNode[5]->m_region : (new BoundingBox((nodePos + vec3(0, -halfDimension.y, 0)), (nodePos + vec3(halfDimension.x, 0, halfDimension.z)), 1.0f));  // min (0,-1,0) max(1,0,1)
	octant[6] = (childrenNode[6] != nullptr) ? childrenNode[6]->m_region : (new BoundingBox((nodePos + vec3(-halfDimension.x, -halfDimension.y, 0)), (nodePos + vec3(0, 0, halfDimension.z)), 1.0f)); // min (-1,-1,0) max(0,0,1)
	octant[7] = (childrenNode[7] != nullptr) ? childrenNode[7]->m_region : (new BoundingBox((m_region->m_min), (nodePos), 1.0f));  // min (-1,-1,-1) max(0,0,0)
	//First, is the item completely contained within the root bounding box?
	//note2: I shouldn't actually have to compensate for this. If an object is out of our predefined bounds, then we have a problem/error.
	//          Wrong. Our initial bounding box for the terrain is constricting its height to the highest peak. Flying units will be above that.
	//             Fix: I resized the enclosing box to 256x256x256. This should be sufficient.
	if (object->m_obb->m_max != object->m_obb->m_min && IsObjInsideBox(object, m_region))
	{
		bool found = false;
		//we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
		for (int a = 0; a<8; a++)
		{
			//is the object fully contained within a quadrant?
			if (IsObjInsideBox(object, octant[a]))//childOctant[a].Contains(Item.BoundingBox) == ContainmentType.Contains)
			{
				if (childrenNode[a] != nullptr)
					childrenNode[a]->InsertObjects(object);   //Add the item into that tree and let the child tree figure out what to do with it
				else
				{
					childrenNode[a] = CreateNode(octant[a], vector<GameObject*>(1,object));//CreateNode(childOctant[a], Item);   //create a new tree node with the item
					m_activeNodes |= (BYTE)(1 << a);
				}
				found = true;
			}
		}
		if (!found)
		{
			object->m_owner = this;

			m_objects.push_back(object);
		}

	}
	//else if (Item.BoundingSphere.Radius != 0 && m_region.Contains(Item.BoundingSphere) == ContainmentType.Contains)
	//{
	//	bool found = false;
	//	//we will try to place the object into a child node. If we can't fit it in a child node, then we insert it into the current node object list.
	//	for (int a = 0; a < 8; a++)
	//	{
	//		//is the object contained within a child quadrant?
	//		if (childOctant[a].Contains(Item.BoundingSphere) == ContainmentType.Contains)
	//		{
	//			if (m_childNode[a] != null)
	//				m_childNode[a].Insert(Item);   //Add the item into that tree and let the child tree figure out what to do with it
	//			else
	//			{
	//				m_childNode[a] = CreateNode(childOctant[a], Item);   //create a new tree node with the item
	//				m_activeNodes |= (byte)(1 << a);
	//			}
	//			found = true;
	//		}
	//	}
	//	if (!found) m_objects.Add(Item);
	//}
	else
	{
		//either the item lies outside of the enclosed bounding box or it is intersecting it. Either way, we need to rebuild
		//the entire tree by enlarging the containing bounding box
		//BoundingBox enclosingArea = FindBox();
		BuildTree();
	}
	for (int i = 0; i < 8; i++)
	{
		if ( childrenNode[i] == nullptr)
		{
			delete octant[i];
			octant[i] = nullptr;
		}
	}
}

//// in BuildTree
/// This finds the dimensions of the bounding box necessary to tightly enclose all items in the object list.
void OcTreeNode::FindEnclosingBox()
{
	vec3 global_min = m_region->m_min, global_max = m_region->m_max;



	//go through all the objects in the list and find the extremes for their bounding areas.
	for each(GameObject* obj in m_objects)
	{
		vec3 local_min = vec3(0,0,0), local_max = vec3(0,0,0);

	//	if (!obj.HasBounds)
		{
			//the object doesn't have any bounding regions associated with it, so we're going to skip it.
			//otherwise, we'll get stack overflow exceptions since we'd be creating an infinite number of nodes approaching zero.
			//continue;
	//		throw new Exception("Every object in the octTree must have a bounding region!");
		}

		if (obj->m_obb != nullptr && obj->m_obb->m_max != obj->m_obb->m_min)
		{
			local_min = obj->m_obb->m_min;
			local_max = obj->m_obb->m_max;
		}

 
		if (local_min.x < global_min.x) global_min.x = local_min.x;
		if (local_min.y < global_min.y) global_min.y = local_min.y;
		if (local_min.z < global_min.z) global_min.z = local_min.z;

		if (local_max.x > global_max.x) global_max.x = local_max.x;
		if (local_max.y > global_max.y) global_max.y = local_max.y;
		if (local_max.z > global_max.z) global_max.z = local_max.z;
	}

	m_region->m_min = global_min;
	m_region->m_max = global_max;
}

/// This finds the smallest enclosing cube which is a power of 2, for all objects in the list.
void 	 OcTreeNode::FindingEnclosingCube()
{
	FindEnclosingBox();
	//find the min offset from (0,0,0) and translate by it for a short while
	vec3 offset = m_region->m_min - vec3(0,0,0);
	m_region->m_min += offset;
	m_region->m_max += offset;

	//find the nearest power of two for the max values
	//int highX = (int)Math.Floor(Math.Max(Math.Max(m_region->m_max.X, m_region->m_max.Y), m_region->m_max.Z));
	int highX = (int)(std::max(std::max(m_region->m_max.x, m_region->m_max.y), m_region->m_max.z));

	//see if we're already at a power of 2
	for (int bit = 0; bit < 32; bit++)
	{
		if (highX == 1 << bit)
		{
			m_region->m_max =   vec3(highX, highX, highX);

			m_region->m_min -= offset;
			m_region->m_max -= offset;
			return;
		}
	}

	//gets the most significant bit value, so that we essentially do a Ceiling(X) with the 
	//ceiling result being to the nearest power of 2 rather than the nearest integer.
	int x = std::pow(2, std::round(std::log(highX) / std::log(2))); //Calc.SigBit(highX);

	m_region->m_max =   vec3(x, x, x);

	m_region->m_min -= offset;
	m_region->m_max -= offset;
}

bool OcTreeNode::CanKill()
{

	if (this->parent == nullptr) return false;  // can not kill the root

	if (this->m_activeNodes == 0 && this->m_objects.size() <= 0)
		return true;
	return false;
}

void OcTreeNode::KillNode(OcTreeNode *target , GameObject* obj)
{
	if (target->CanKill())
	{

		OcTreeNode *P = target->parent;

		for (int i = 0; i < 8; i++)
		{
			if (P->childrenNode[i] == target)
			{
				P->m_activeNodes ^= (BYTE)(1 << i);
				delete target;
				target = nullptr;
				P->childrenNode[i] = nullptr;
 				break;
			}

		}
		KillNode(P, obj);
	}

}
 
OcTreeNode::~OcTreeNode()
{

	// what obj s are contained in THIS PARTICULAR octree node

	delete m_region; // draw this node}
}



vector<IntersectInfo*> OcTreeNode::GetIntersectionList(vector<GameObject *> &parentObjects)
{
	vector<IntersectInfo*> resList;

	// assume all parent objects have been processed. and we only care about local objects vs parents objects
	for each(GameObject *pObj in parentObjects)
	{
		for each(GameObject* lObj in m_objects)
		{
			IntersectInfo *temp = CollisionDetection::checkIntersection(pObj, lObj);
			if (temp != nullptr)
			{
				resList.push_back(temp);
			}
		}
	
	}

	if (m_objects.size()>1)  //local vs local
	{
		// collision inside the node
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			for (auto it2 = it + 1; it2 != m_objects.end(); ++it2)
			{
				IntersectInfo *temp = CollisionDetection::checkIntersection(*it, *it2);
				if (temp != nullptr)
				{
					resList.push_back(temp);
				}
			}

		}
	}
 
	// send parent objects + local objects to next children node
	// merge the objets
	for each(GameObject* lObj in m_objects)
	{
		parentObjects.push_back(lObj);
	}

	//get the intersection list from children
	for each(OcTreeNode* child in childrenNode)
	{
		if (child != nullptr)
		{
			vector<IntersectInfo*>  ListFormChild = child->GetIntersectionList(parentObjects);
			resList.insert(resList.end(), ListFormChild.begin(), ListFormChild.end()); //merge two list
		}
	}
	return resList;
}
