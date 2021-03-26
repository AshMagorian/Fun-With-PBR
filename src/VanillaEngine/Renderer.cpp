#include "VanillaEngine.h"
#include "Model.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::OnInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, std::shared_ptr<PBR_Material> _mat)
{
	m_shaderProgram = _shader;
	m_va = _va;
	m_pbrMat = _mat;
	m_cam = GetApplication()->GetCamera();
}

void Renderer::OnInit(std::string _path, std::shared_ptr<ShaderProgram> _shader)
{
	//Convert the path to char*
	char* p = &_path[0];
	m_assimpModel = std::make_shared<Model>(p);
	m_shaderProgram = _shader;
	m_cam = GetApplication()->GetCamera();
}

void Renderer::OnTick()
{
	if (m_shaderProgram)
	{
		m_shaderProgram->SetUniform("in_Projection", m_cam->GetProjectionMatrix());
		m_shaderProgram->SetUniform("in_View", m_cam->GetViewMatrix());
	}
}
void Renderer::OnDisplay()
{
	if (m_shaderProgram)
	{
		m_shaderProgram->SetUniform("in_Model", GetEntity()->GetTransform()->GetModelMatrix());
		m_shaderProgram->SetUniform("in_NormalMtx", GetEntity()->GetTransform()->GetNormalMatrix());
		if (m_va)
		{
			if (m_pbrMat)
			{
				BindPBRValues();
			}
			m_shaderProgram->Draw(m_va);
		}
		else if (m_assimpModel)
		{
			//Render using assimp
			BindIBLMaps();
			m_assimpModel->Draw(m_shaderProgram);
		}
	}
	else
	{
		std::cout << "Couldn't draw, shader not found" << std::endl;
	}
}

void Renderer::BindPBRValues()
{
	int matBinary = 0;

	if (m_pbrMat->GetAlbedo() != nullptr) {
		m_shaderProgram->SetUniform("in_Material.texture_diffuse1", m_pbrMat->GetAlbedo());
		matBinary += 16;
	}
	else
		m_shaderProgram->SetUniform("in_Tex.albedo", m_pbrMat->GetAlbedoValue());

	if (m_pbrMat->GetNormal() != nullptr) {
		m_shaderProgram->SetUniform("in_Material.texture_normal1", m_pbrMat->GetNormal());
		matBinary += 8;
	}
	else
		m_shaderProgram->SetUniform("in_Tex.normal", glm::vec3(0.0f, 0.0f, 1.0f));

	if (m_pbrMat->GetMetallic() != nullptr) {
		m_shaderProgram->SetUniform("in_Material.texture_metallic1", m_pbrMat->GetMetallic());
		matBinary += 4;
	}
	else
		m_shaderProgram->SetUniform("in_Tex.metallic", m_pbrMat->GetMetallicValue());

	if (m_pbrMat->GetRoughness() != nullptr) {
		m_shaderProgram->SetUniform("in_Material.texture_roughness1", m_pbrMat->GetRoughness());
		matBinary += 2;
	}
	else
		m_shaderProgram->SetUniform("in_Tex.roughness", m_pbrMat->GetRoughnessValue());

	if (m_pbrMat->GetAO() != nullptr) {
		m_shaderProgram->SetUniform("in_Material.texture_ao1", m_pbrMat->GetAO());
		matBinary += 1;
	}
	else
		m_shaderProgram->SetUniform("in_Tex.ao", 1.0f);

	if (m_pbrMat->GetDisplacement() != nullptr) {
		m_shaderProgram->SetUniform("in_Material.texture_displacement1", m_pbrMat->GetDisplacement());

		m_shaderProgram->SetUniform("in_Parallax_ClipBorders", (int)m_parallax_clipBorders);
		m_shaderProgram->SetUniform("in_Parallax_Height", m_parallax_height);
		m_shaderProgram->SetUniform("in_Parallax_MinLayers", m_parallax_minLayers);
		m_shaderProgram->SetUniform("in_Parallax_MaxLayers", m_parallax_maxLayers);
		matBinary += 32;
	}

	m_shaderProgram->SetUniform("in_MatBinary", matBinary);
	m_shaderProgram->SetUniform("in_TexCoordScale", m_texCoordScale);
	BindIBLMaps();
}

void Renderer::BindIBLMaps()
{
	std::string mapName = GetApplication()->GetSkybox()->GetCurrentMapName();
	
	glUseProgram(m_shaderProgram->GetId());
	glUniform1i(glGetUniformLocation(m_shaderProgram->GetId(), "irradianceMap"), 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PBR_Material::GetIrradiance(mapName));
	
	glUniform1i(glGetUniformLocation(m_shaderProgram->GetId(), "prefilterMap"), 7);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PBR_Material::GetPrefilter(mapName));
	
	glUniform1i(glGetUniformLocation(m_shaderProgram->GetId(), "brdfLUT"), 8);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, PBR_Material::GetBRDF());
}