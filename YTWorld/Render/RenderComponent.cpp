#include "RenderComponent.h"


void RenderComponent::InitGLdata(const char* objFilePath, const char* ddsFilePath, const char* vertexShader, const char* fragmentShader)
{
	createShader(vertexShader, fragmentShader);
	createMesh(objFilePath);

	createMaterial(ddsFilePath);

	glBindVertexArray(0);

}

void RenderComponent::createMesh(const char* objFilePath)
{
	if (m_mesh == nullptr)
		m_mesh = new Mesh(objFilePath);
}
void RenderComponent::createShader(const char* vertexShader, const char* fragmentShader)
{
	if (m_shader == nullptr)
		m_shader = new Shader(vertexShader, fragmentShader);
}
void RenderComponent::createMaterial(const char* ddsFilePath)
{
	if (m_material == nullptr)
		m_material = new Material(ddsFilePath);
}

void RenderComponent::Render(glm::mat4 matrix)
{

	if (m_mesh == nullptr) return;

	//////// draw bounding box

	if (m_shader != nullptr)
	{
		m_shader->Bind();

		m_shader->UpdateShader(matrix);
	}
	if (m_material != nullptr)
	{
		m_material->Bind();

	}
	m_mesh->Draw();
}


