#include "Mesh.h"
 

std::map<std::string, Mesh> Mesh::existList;


Mesh::Mesh( const char* objFilePath)
{
	InitMesh(objFilePath);
 }
void Mesh::InitMesh(const char* objFilePath)
{
	if (existList.find(objFilePath) != existList.end())
	{
		m_vertexArrayObjID = existList[objFilePath].m_vertexArrayObjID;
		m_numVertices = existList[objFilePath].m_numVertices;
		m_numCoords = existList[objFilePath].m_numCoords;
		m_numNormals = existList[objFilePath].m_numNormals;

		min = existList[objFilePath].min;
		max = existList[objFilePath].max;

		return; // obj has already rigister

	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.

	GLuint buffers[2];

	bool res = loadOBJ(objFilePath, vertices, uvs, normals);

	///// get bounding box info
	min = vertices[0];
	max = vertices[0];
	for (int i = 0; i < vertices.size(); ++i)
	{
		min.x = std::min( min.x , vertices[i].x);
		min.y = std::min(min.y, vertices[i].y);
		min.z = std::min(min.z, vertices[i].z);
		max.x = std::max(max.x, vertices[i].x);
		max.y = std::max(max.y, vertices[i].y);
		max.z = std::max(max.z, vertices[i].z);
	}
 

 
	//GLuint	objVertexArrayID;
	glGenVertexArrays(1, &m_vertexArrayObjID);
	glBindVertexArray(m_vertexArrayObjID);

	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind buffer for colors and copy data into buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


	std::vector<GLuint> temp;
	temp.push_back(m_vertexArrayObjID);
	temp.push_back(vertices.size());
	temp.push_back(uvs.size());
	temp.push_back(normals.size());

	//m_vertexArrayObjID = objVertexArrayID;
	m_numVertices = vertices.size();
	m_numCoords = uvs.size();
	m_numNormals = normals.size();

	glBindVertexArray(0);
	existList[objFilePath] = *this;
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObjID);
 
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

	glBindVertexArray(0);
}


Mesh::~Mesh()
{
	//	glDeleteBuffers(2, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObjID);
}
