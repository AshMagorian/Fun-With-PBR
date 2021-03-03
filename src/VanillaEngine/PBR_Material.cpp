#include "PBR_Material.h"
#include "ShaderProgram.h"
#include <glm/ext.hpp>

GLuint PBR_Material::m_irradianceMapID = 0;
GLuint PBR_Material::m_prefilterMapID = 0;
GLuint PBR_Material::m_brdfID = 0;
bool PBR_Material::brdfCheck = false;

void PBR_Material::SetTextures(std::shared_ptr<Texture> _albedo,
								std::shared_ptr<Texture> _normal,
								std::shared_ptr<Texture> _metallic,
								std::shared_ptr<Texture> _roughness,
								std::shared_ptr<Texture> _ao)
{
	m_albedo = _albedo;
	m_normal = _normal;
	m_metallic = _metallic;
	m_roughness = _roughness;
	m_ao = _ao;
}

void PBR_Material::SetPrefilter(GLuint _id)
{
	m_prefilterMapID = _id;
	if (brdfCheck == false)
	{
		MakeBRDFTex();
		brdfCheck = true;
	}
}

void PBR_Material::MakeBRDFTex()
{
	std::shared_ptr<ShaderProgram> brdfShader = std::make_shared<ShaderProgram>("../src/resources/shaders/brdf.vert", "../src/resources/shaders/brdf.frag");
	unsigned int captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glGenTextures(1, &m_brdfID);
	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, m_brdfID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdfID, 0);
	glViewport(0, 0, 512, 512);

	glUseProgram(brdfShader->GetId());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//render quad here
	unsigned int emptyVA;
	glGenVertexArrays(1, &emptyVA);
	glBindVertexArray(emptyVA);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}