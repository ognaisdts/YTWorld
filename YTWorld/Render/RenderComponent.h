#pragma once
#include "..\Geometry\Mesh.h"
#include "..\Geometry\Material.h"
#include "..\Geometry\Shader.h"

#include "glm\glm.hpp"
class RenderComponent
{
public:
	Mesh *m_mesh;
	Shader *m_shader;
	Material *m_material;
	bool m_isSky;
	RenderComponent(
		const char* objfile,
		const char* ddsfile,
		const char* vertexShader,
		const char* fragmentShader,
		bool _isSky = false
	)
	{
		InitGLdata(objfile, ddsfile, vertexShader, fragmentShader);
		m_isSky = _isSky;
	}

	void InitGLdata(const char* objFilePath, const char* ddsFilePath, const char* vertexShader, const char* fragmentShader);
	void createMesh(const char* ddsFilePath);
	void createShader(const char* vertexShader, const char* fragmentShader);
	void createMaterial(const char* ddsFilePath);

	void Render(glm::mat4 matrix);
};