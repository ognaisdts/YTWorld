#include "CollisionDetection.h"

int CollisionDetection::collicount = 0;
bool CollisionDetection::calculate(vec3 T, vec3 L, vec3 Ax, vec3 Ay, vec3 Az, vec3 Bx, vec3 By, vec3 Bz,
	float wa, float ha, float da, float wb, float hb, float db)
{
	float TdotL = abs(glm::dot(T, L));

	float leftSide = abs(dot(wa*Ax, L)) + abs(dot(ha*Ay, L)) + abs(dot(da*Az, L)) +
		abs(dot(wb*Bx, L)) + abs(dot(hb*By, L)) + abs(dot(db*Bz, L));

	return (TdotL > leftSide) ? true : false;

}

float CollisionDetection::calculateS(vec3 T, vec3 L, vec3 Ax, vec3 Ay, vec3 Az, vec3 Bx, vec3 By, vec3 Bz,
	float wa, float ha, float da, float wb, float hb, float db)
{
	float TdotL = abs(glm::dot(T, L));

	float rightSide = abs(dot(wa*Ax, L)) + abs(dot(ha*Ay, L)) + abs(dot(da*Az, L)) +
		abs(dot(wb*Bx, L)) + abs(dot(hb*By, L)) + abs(dot(db*Bz, L));

	return (TdotL - rightSide);
}

/*
bool CollisionDetection::SATcheck(GameObject &obj1, GameObject &obj2)
{
	//////
	//PA = coordinate position of the center of A
	vec3 PA = vec3(obj1.getModelToWorldMatrix()* vec4(obj1.m_obb->m_center, 1));//obj1.position;
	//Ax = unit vector representing the x - axis of A
	vec3 Ax = vec3((obj1.getModelToWorldMatrix())[0]);
	//Ay = unit vector representing the y - axis of A
	vec3 Ay = vec3((obj1.getModelToWorldMatrix())[1]);
	//Az = unit vector representing the z - axis of A
	vec3 Az = cross(Ax, Ay);//vec3((obj1.getModelToWorldMatrix())[2]);
	//WA = half width of A(corresponds with the local x - axis of A)
	float WA = obj1.m_obb->m_size.x / 2;
	//HA = half height of A(corresponds with the local y - axis of A)
	float HA = obj1.m_obb->m_size.y / 2;
	//DA = half depth of A(corresponds with the local z - axis of A)
	float DA = obj1.m_obb->m_size.z / 2;

	//PB = coordinate position of the center of B
	vec3 PB = vec3(obj2.getModelToWorldMatrix()* vec4(obj2.m_obb->m_center, 1)); //obj2.position;
	//Bx = unit vector representing the x - axis of B
	vec3 Bx = vec3((obj2.getModelToWorldMatrix())[0]);
	//By = unit vector representing the y - axis of B
	vec3 By = vec3((obj2.getModelToWorldMatrix())[1]);
	//Bz = unit vector representing the z - axis of B
	vec3 Bz = cross(Bx, By);//vec3((obj2.getModelToWorldMatrix())[2]);
	//WB = half width of B(corresponds with the local x - axis of B)
	float WB = obj2.m_obb->m_size.x / 2;
	//HB = half height of B(corresponds with the local y - axis of B)
	float HB = obj2.m_obb->m_size.y / 2;
	//DB = half depth of B(corresponds with the local z - axis of B)
	float DB = obj2.m_obb->m_size.z / 2;

	normalize(Ax);
	normalize(Ay);
	normalize(Az);
	normalize(Bx);
	normalize(By);
	normalize(Bz);

	//T = PB – PA
	vec3 T = PB - PA;
	//Rij = Ai •Bj


	vec3 Lset[] = { Ax, Ay, Az, Bx, By, Bz, cross(Ax, Bx), cross(Ax, By), cross(Ax, Bz),
		cross(Ay, Bx), cross(Ay, By), cross(Ay, Bz),
		cross(Az, Bx), cross(Az, By), cross(Az, Bz)
	};
	for (int i = 0; i < 15; ++i)
	{
		normalize(Lset[i]);
		if (calculate(T, Lset[i], Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB)) return true;
	}

	return false;
	//	|Tdot L | > | W Ax dot L |+ | H Ay dot L | +| D Az dot L |+ | W Bx L | +| H By L |+ | D Bz L| 

}
*/
void CollisionDetection::ElasticCollision(GameObject *obj1, GameObject *obj2, float coef)
{

	obj1->m_velocity = (obj1->m_mass*obj1->m_velocity + obj2->m_mass*obj2->m_velocity + coef *obj2->m_mass*(obj2->m_velocity - obj1->m_velocity)) / (obj1->m_mass + obj2->m_mass);
	obj2->m_velocity = (obj1->m_mass*obj1->m_velocity + obj2->m_mass*obj2->m_velocity +  coef *obj1->m_mass*(obj1->m_velocity - obj2->m_velocity)) / (obj1->m_mass + obj2->m_mass);
}

bool CollisionDetection::checkFaceAxis(int &AxisNo, int curAxisNo, float &sMax, float curS, vec3 &normal, vec3 curNormal)
{
	if (curS > 0) return true;

	if (curS > sMax)
	{
		AxisNo = curAxisNo;
		sMax = curS;
		normal = curNormal;
	}

	return false;
}

bool CollisionDetection::checkEdgeAxis(int &AxisNo, int curAxisNo, float &sMax, float curS, vec3 &normal, vec3 curNormal)
{
	if (curS > 0) return true;

	float len = 1.0f / sqrtf(abs(dot(curNormal, curNormal)));
	curS *= len;

	if (curS > sMax)
	{
		AxisNo = curAxisNo;
		sMax = curS;
		normal = curNormal*len;
	}

	return false;
}


IntersectInfo* CollisionDetection::checkIntersection(GameObject *object, GameObject *other)
{


	//////
	//PA = coordinate position of the center of A
	vec3 PA = vec3(object->getModelToWorldMatrix()* vec4(object->m_obb->m_center, 1));//obj1.position;
	//Ax = unit vector representing the x - axis of A
	vec3 Ax = vec3((object->getModelToWorldMatrix())[0]);
	//Ay = unit vector representing the y - axis of A
	vec3 Ay = vec3((object->getModelToWorldMatrix())[1]);
	//Az = unit vector representing the z - axis of A
	vec3 Az = cross(Ax, Ay);//vec3((object->getModelToWorldMatrix())[2]);
	//WA = half width of A(corresponds with the local x - axis of A)
	float WA = object->m_obb->m_size.x / 2;
	//HA = half height of A(corresponds with the local y - axis of A)
	float HA = object->m_obb->m_size.y / 2;
	//DA = half depth of A(corresponds with the local z - axis of A)
	float DA = object->m_obb->m_size.z / 2;

	//PB = coordinate position of the center of B
	vec3 PB = vec3(other->getModelToWorldMatrix()* vec4(other->m_obb->m_center, 1)); //obj2.position;
	//Bx = unit vector representing the x - axis of B
	vec3 Bx = vec3((other->getModelToWorldMatrix())[0]);
	//By = unit vector representing the y - axis of B
	vec3 By = vec3((other->getModelToWorldMatrix())[1]);
	//Bz = unit vector representing the z - axis of B
	vec3 Bz = cross(Bx, By);// vec3((other->getModelToWorldMatrix())[2]);
	//WB = half width of B(corresponds with the local x - axis of B)
	float WB = other->m_obb->m_size.x / 2;
	//HB = half height of B(corresponds with the local y - axis of B)
	float HB = other->m_obb->m_size.y / 2;
	//DB = half depth of B(corresponds with the local z - axis of B)
	float DB = other->m_obb->m_size.z / 2;

	normalize(Ax);
	normalize(Ay);
	normalize(Az);
	normalize(Bx);
	normalize(By);
	normalize(Bz);

	//T = PB – PA
	vec3 T = PB - PA;
	//Rij = Ai •Bj



	/////////
	// skip parallel check
	////////
	
	//// assume has 3 kind of normal
	vec3 nA;
	vec3 nB;
	vec3 nE;
	float aMax = INT_MIN;
	float bMax = INT_MIN;
	float eMax = INT_MIN;
	int aAxisNo, bAxisNo, eAxisNo;
	float s; // s =  |TdotL| - (| W Ax dot L |+ | H Ay dot L | +| D Az dot L |+ | W Bx L | +| H By L |+ | D Bz L| )

	//vec3 Lset[] = { Ax, Ay, Az, Bx, By, Bz, cross(Ax, Bx), cross(Ax, By), cross(Ax, Bz),
	//	cross(Ay, Bx), cross(Ay, By), cross(Ay, Bz),
	//	cross(Az, Bx), cross(Az, By), cross(Az, Bz)
	//};

	// Ax
	s = calculateS(T, Ax, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkFaceAxis(aAxisNo, 0, aMax,  (s), nA, Ax)) return nullptr;
	//Ay
	s = calculateS(T, Ay, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkFaceAxis(aAxisNo, 1, aMax,  (s), nA, Ay)) return nullptr;
	//Az
	s = calculateS(T, Az, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkFaceAxis(aAxisNo, 2, aMax,  (s), nA, Az))return nullptr;

	//Bx
	s = calculateS(T, Bx, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkFaceAxis(bAxisNo, 3, bMax,  (s), nB, Bx))return nullptr;
	//By
	s = calculateS(T, By, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkFaceAxis(bAxisNo, 4, bMax,  (s), nB, By))return nullptr;
	//Bz
	s = calculateS(T, Bz, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkFaceAxis(bAxisNo, 5, bMax,  (s), nB, Bz))return nullptr;


	vec3 L = vec3(0, 0, 0);

	// cross(Ax, Bx)
	L = cross(Ax, Bx);
	normalize(L);
	
	s = calculateS(T, L, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkEdgeAxis(eAxisNo, 6, eMax,  (s), nE, L))return nullptr;
	if (eAxisNo == 6 && cross(Ax,Bx) == vec3(0,0,0)) eMax = INT_MIN;
	//cross(Ax, By)
	L = cross(Ax, By);
	//normalize(L);
	s = calculateS(T, L, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkEdgeAxis(eAxisNo, 7, eMax,  (s), nE, L))return nullptr;
	if (eAxisNo == 7 && cross(Ax, By) == vec3(0, 0, 0)) eMax = INT_MIN;

	//cross(Ax, Bz)
	L = cross(Ax, Bz);
	normalize(L);
	s = calculateS(T, L, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkEdgeAxis(eAxisNo, 8, eMax,  (s), nE, L))return nullptr;
	if (eAxisNo == 8 && cross(Ax, Bz) == vec3(0, 0, 0)) eMax = INT_MIN;

	//cross(Ay, Bx)
	L = cross(Ay, Bx);
	normalize(L);
	s = calculateS(T, L, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkEdgeAxis(eAxisNo, 9, eMax,  (s), nE, L))return nullptr;
	if (eAxisNo == 9 && cross(Ay, Bx) == vec3(0, 0, 0)) eMax = INT_MIN;

	//cross(Ay, By)
	L = cross(Ay, By);
	normalize(L);
	s = calculateS(T, L, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkEdgeAxis(eAxisNo, 10, eMax,  (s), nE, L))return nullptr;
	if (eAxisNo == 10 && cross(Ay, By) == vec3(0, 0, 0)) eMax = INT_MIN;

	//cross(Ay, Bz)
	L = cross(Ay, Bz);
	normalize(L);
	s = calculateS(T, L, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkEdgeAxis(eAxisNo, 11, eMax,  (s), nE, L))return nullptr;
	if (eAxisNo == 11 && cross(Ay, Bz) == vec3(0, 0, 0)) eMax = INT_MIN;

	//cross(Az, Bx)
	L = cross(Az, Bx);
	normalize(L);
	s = calculateS(T, L, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkEdgeAxis(eAxisNo, 12, eMax,  (s), nE, L))return nullptr;
	if (eAxisNo == 12 && cross(Az, Bx) == vec3(0, 0, 0)) eMax = INT_MIN;

	//cross(Az, By)
	L = cross(Az, By);
	normalize(L);
	s = calculateS(T, L, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkEdgeAxis(eAxisNo, 13, eMax,  (s), nE, L))return nullptr;
	if (eAxisNo == 13 && cross(Az, By) == vec3(0, 0, 0)) eMax = INT_MIN;

	//cross(Az, Bz)
	L = cross(Az, Bz);
	normalize(L);
	s = calculateS(T, L, Ax, Ay, Az, Bx, By, Bz, WA, HA, DA, WB, HB, DB);
	if (checkEdgeAxis(eAxisNo, 14, eMax,  (s), nE, L))return nullptr;
	if (eAxisNo == 14 && cross(Az, Bz) == vec3(0, 0, 0))  eMax = INT_MIN;
	// ^^^ this is for miss cross  situation  ^^^  normal might become vec3(0,0,0)....  we wont like it happened.


	// Artificial axis bias to improve frame coherence
	// compare A B and E
	const float kRelTol = 0.95f;
	const float kAbsTol = 0.01f;

	int axisNo;
	float sMax;
	vec3 normal;
	float faceMax = std::max(aMax, bMax);

	if (eMax * kRelTol > faceMax + kAbsTol)
	{
		axisNo = eAxisNo;
		normal = nE;
		sMax = eMax;
	}
	else
	{
		if (kRelTol* bMax > aMax * kAbsTol)
		{
			axisNo = bAxisNo;
			normal = nB;
			sMax = bMax;
		}
		else
		{
			axisNo = aAxisNo;
			normal = nA;
			sMax = aMax;
		}

	}

	///// if  A -> B   dot  <-n <0 ..... flip n =>   A->B     ->n
 	if (dot(normal, T) < 0) normal *= (-1);

	// if axisNo <6  can clip and get more information ... future might use......


	IntersectInfo *ret = new IntersectInfo();

	ret->hasHit = true; 

	// right now.... just care first one
	ret->manifold.contacts[0].normal = normal;
	ret->manifold.contacts[0].penetration = abs(sMax);
	ret->manifold.contactCount = axisNo;

	if (ret->hasHit)
	{
		ret->manifold.objA = object;
		ret->manifold.objB = other;
	}
	else
	{
		delete ret;
		ret =  nullptr;
	}
	return ret;
 }


void CollisionDetection::processCollisionRespond(std::vector<IntersectInfo*> IntersectionList, float deltaTime)
{
//	if (IntersectionList.size() >0)
//	std::cout << "collision happen : "<< IntersectionList.size() << std::endl;

	for each (IntersectInfo *info in IntersectionList)
	{
		//positon adjust;
		GameObject *A = info->manifold.objA;
		GameObject *B = info->manifold.objB;
		vec3 normal = info->manifold.contacts[0].normal;
		float penetration = info->manifold.contacts[0].penetration;

		////Impulse
		vec3 relVel = B->m_velocity - A->m_velocity;
		float contactVel = dot(relVel, normal);
		if (contactVel > 0) return;
		float coe = 0;
		float J = -(1.0f + coe)*contactVel;
		J /=(1.0f/ A->m_mass) + (1.0f/B->m_mass);

		vec3 Impluse = J * normal;
		A->m_velocity -= (1.0f / A->m_mass)*Impluse;
		B->m_velocity += (1.0f / B->m_mass)*Impluse;
		//ElasticCollision(A, B, 0);  instead abrove

		//Penetration Resolution
		const float k_slop = 0.01f; // Penetration allowance
		const float percent = 0.8f; // Penetration percentage to correct
		vec3 correction = (std::max(penetration - k_slop, 0.0f) / ((1.0f / A->m_mass) + (1.0f / B->m_mass))) * percent * normal;
		A->position -= (1.0f / A->m_mass)* correction;
		B->position += (1.0f / B->m_mass) * correction;


		cout << "normal " << info->manifold.contacts[0].normal.x << ", " << info->manifold.contacts[0].normal.y << ", " << info->manifold.contacts[0].normal.z << endl;
		//cout << "penetration " << info->manifold.contacts[0].penetration << endl;
		cout << "no : " << info->manifold.contactCount << endl;
	}

}

//
//CollisionDetection::CollisionDetection()
//{
//}
//
//
//CollisionDetection::~CollisionDetection()
//{
//}
