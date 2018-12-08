#include "Material.h"


Material::Material(const char* ddsFilePath)
{
	InitTexture(ddsFilePath);
}

void Material::InitTexture(const char* ddsFilePath)
{
	m_TextureID = loadDDS(ddsFilePath);//("Asset/Crate_1.DDS");

}

void Material::Bind()
{
 
	 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		// Set our "myTextureSampler" sampler to user Texture Unit 0

}

Material::~Material()
{
	glDeleteTextures(1, &m_TextureID);
}
