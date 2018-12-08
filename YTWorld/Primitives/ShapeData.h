#pragma once
#include <GL\glew.h>
#include "Vertex.h"

struct ShapeData
{
	//constructor  safe
	ShapeData() : numVertices(0), numIndices(0), vertices(nullptr), indices(nullptr)
	{}

	Vertex *vertices;
	GLuint numVertices;
	GLushort *indices;
	GLuint numIndices;

	GLsizeiptr vertexBufferSize() const  // GLsizeiptr = int
	{
		return numVertices * sizeof(Vertex);
	}

	GLsizeiptr indexBufferSize() const
	{
		return numIndices * sizeof(GLshort);
	}

	//~ShapeData()
	//{
	//	delete []vertices;
	//	delete [] indices;
	//	numIndices = numVertices = 0; 
	//}

	void cleanup()
	{
			delete []vertices;
			delete [] indices;
			numIndices = numVertices = 0; 
	}
};