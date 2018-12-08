#include "BoundingBox.h"

Shader* BoundingBox::s_shader;// = new Shader("TransformVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

BoundingBox::BoundingBox(glm::vec3 _min, glm::vec3 _max, float _width)
	:m_min(_min),
	m_max(_max),
	m_width(_width)
{
	m_size = glm::vec3(m_max.x - m_min.x, m_max.y - m_min.y, m_max.z - m_min.z);
	m_center = glm::vec3((m_min.x + m_max.x) / 2, (m_min.y + m_max.y) / 2, (m_min.z + m_max.z) / 2);
	m_transform = glm::translate(glm::mat4(1), m_center)* glm::scale(m_size);

	if (s_shader == nullptr)
	 s_shader = new Shader("TransformVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	m_shader = s_shader;//new Shader("TransformVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	//// init buffer
	// Cube 1x1x1, centered on origin
	GLfloat vertices[] = {
		-0.5, -0.5, -0.5, //1.0,
		0.5, -0.5, -0.5,// 1.0,
		0.5, 0.5, -0.5,// 1.0,
		-0.5, 0.5, -0.5, //1.0,
		-0.5, -0.5, 0.5, //1.0,
		0.5, -0.5, 0.5, //1.0,
		0.5, 0.5, 0.5, //1.0,
		-0.5, 0.5, 0.5,// 1.0,
	};
	GLushort elements[] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7
	};

 
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 
//	glBindAttribLocation(m_shaderID, 0, "vertexPosition_modelspace");
//	m_uniforms = glGetUniformLocation(m_shaderID, "fullTransformMatrix");
}
void BoundingBox::drawBox(  glm::mat4 _fuuMatrix )
{
	glUseProgram(m_shader->m_ProgramID);
	/* Apply object's transformation matrix */
	glm::mat4 m = _fuuMatrix*m_transform;
	glUniformMatrix4fv(m_shader->m_uniforms[0], 1, GL_FALSE, &m[0][0]);


	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,  // attribute
		3,                  // number of elements per vertex, here (x,y,z,w)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);
	glLineWidth(m_width);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 


}


BoundingBox::~BoundingBox()
{
	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &ibo_elements);
 //	delete m_shader;
}
void BoundingBox::PrintInfo()
{
	std::cout << "min  ( " << m_min.x << ", " << m_min.y << ", " << m_min.z << " )" << std::endl;
	std::cout << "max  ( " << m_max.x << ", " << m_max.y << ", " << m_max.z << " )" << std::endl;



}
