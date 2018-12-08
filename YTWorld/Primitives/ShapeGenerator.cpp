#include "ShapeGenerator.h"
#include <glm\glm.hpp> // glm::mat4
#include "Vertex.h"
#define NUM_ARRAY_ELEMENTS(a)  sizeof(a) / sizeof(*a);
using glm::vec3;
 
ShapeData ShapeGenerator::makeTriangle()
{
	ShapeData res;

	Vertex myTri[] =
	{
		glm::vec3(0, 1, 0),
		glm::vec3(1, 0, 0),

		glm::vec3(-1, -1, 0),
		glm::vec3(0, 1, 0),

		glm::vec3(1, -1, 0),
		glm::vec3(0, 0, 1),
	};
	res.numVertices = sizeof(myTri) / sizeof(*myTri);
	res.vertices = new Vertex[res.numVertices];
	 memcpy(res.vertices, myTri, sizeof(myTri));

	 GLushort indices[] = { 0, 1, 2 };
	res.numIndices = sizeof(indices) / sizeof(*indices);
	res.indices = new GLushort[res.numIndices];
	memcpy(res.indices, indices, sizeof(indices));
	return res;
}

ShapeData ShapeGenerator::makeCube(){
	ShapeData ret;
	Vertex stackVerts[] =
	{
		vec3(-1.0f, +1.0f, +1.0f), // 0
		vec3(+1.0f, +0.0f, +0.0f),

		vec3(+1.0f, +1.0f, +1.0f), // 1
		vec3(+0.0f, +1.0f, +0.0f),

		vec3(+1.0f, +1.0f, -1.0f), // 2
		vec3(+0.0f, +0.0f, +1.0f),

		vec3(-1.0f, +1.0f, -1.0f), // 3
		vec3(+1.0f, +1.0f, +1.0f),

		vec3(-1.0f, +1.0f, -1.0f), // 4
		vec3(+1.0f, +0.0f, +1.0f),

		vec3(+1.0f, +1.0f, -1.0f), // 5
		vec3(+0.0f, +0.5f, +0.2f),

		vec3(+1.0f, -1.0f, -1.0f), // 6
		vec3(+0.8f, +0.6f, +0.4f),

		vec3(-1.0f, -1.0f, -1.0f), // 7
		vec3(+0.3f, +1.0f, +0.5f),

		vec3(+1.0f, +1.0f, -1.0f), // 8
		vec3(+0.2f, +0.5f, +0.2f),

		vec3(+1.0f, +1.0f, +1.0f), // 9
		vec3(+0.9f, +0.3f, +0.7f),

		vec3(+1.0f, -1.0f, +1.0f), // 10
		vec3(+0.3f, +0.7f, +1.0f),

		vec3(+1.0f, -1.0f, -1.0f), // 11
		vec3(+0.5f, +0.7f, +0.5f),

		vec3(-1.0f, +1.0f, +1.0f), // 12
		vec3(+0.7f, +0.8f, +0.2f),

		vec3(-1.0f, +1.0f, -1.0f), // 13
		vec3(+0.5f, +0.7f, +0.3f),

		vec3(-1.0f, -1.0f, -1.0f), // 14
		vec3(+0.4f, +0.7f, +0.7f),

		vec3(-1.0f, -1.0f, +1.0f), // 15
		vec3(+0.2f, +0.5f, +1.0f),

		vec3(+1.0f, +1.0f, +1.0f), // 16
		vec3(+0.6f, +1.0f, +0.7f),

		vec3(-1.0f, +1.0f, +1.0f), // 17
		vec3(+0.6f, +0.4f, +0.8f),

		vec3(-1.0f, -1.0f, +1.0f), // 18
		vec3(+0.2f, +0.8f, +0.7f),

		vec3(+1.0f, -1.0f, +1.0f), // 19
		vec3(+0.2f, +0.7f, +1.0f),

		vec3(+1.0f, -1.0f, -1.0f), // 20
		vec3(+0.8f, +0.3f, +0.7f),

		vec3(-1.0f, -1.0f, -1.0f), // 21
		vec3(+0.8f, +0.9f, +0.5f),

		vec3(-1.0f, -1.0f, +1.0f), // 22
		vec3(+0.5f, +0.8f, +0.5f),

		vec3(+1.0f, -1.0f, +1.0f), // 23
		vec3(+0.9f, +1.0f, +0.2f),
	};

	ret.numVertices = NUM_ARRAY_ELEMENTS(stackVerts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, stackVerts, sizeof(stackVerts));

	GLushort stackIndices[] = {
		0, 1, 2, 0, 2, 3, // Top
		4, 5, 6, 4, 6, 7, // Front
		8, 9, 10, 8, 10, 11, // Right
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(stackIndices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, stackIndices, sizeof(stackIndices));

	return ret;
}