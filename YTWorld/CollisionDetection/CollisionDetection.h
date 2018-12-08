#pragma once
#include <glm/glm.hpp> 
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

#include <stdlib.h>
#include "..\GameObject.h"
#include "OcTreeNode.h"


using namespace glm;

class GameObject;
struct IntersectInfo;


 

class CollisionDetection
{
public:
	CollisionDetection(){}
	~CollisionDetection(){}
static 	bool calculate(vec3 T, vec3 L, vec3 Ax, vec3 Ay, vec3 Az, vec3 Bx, vec3 By, vec3 Bz,
		float wa, float ha, float da, float wb, float hb, float db);


static float calculateS(vec3 T, vec3 L, vec3 Ax, vec3 Ay, vec3 Az, vec3 Bx, vec3 By, vec3 Bz,
	float wa, float ha, float da, float wb, float hb, float db);

static bool checkFaceAxis(int &AxisNo, int curAxisNo, float &sMax, float curS, vec3 &normal, vec3 curNormal);
static bool checkEdgeAxis(int &AxisNo, int curAxisNo, float &sMax, float curS, vec3 &normal, vec3 curNormal);

//static	bool SATcheck(GameObject &obj1, GameObject &obj2);
static int collicount;

static void ElasticCollision(GameObject *obj1, GameObject *obj2, float coef);

static IntersectInfo* checkIntersection(GameObject *object, GameObject *other);
static void processCollisionRespond(std::vector<IntersectInfo*> IntersectionList, float deltaTime);
};


