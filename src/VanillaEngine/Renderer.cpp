#include "VanillaEngine.h"
#include "Model.h"

Renderer::Renderer() {}
Renderer::~Renderer() {}

void Renderer::OnInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, std::shared_ptr<PBR_Material> _mat)
{
	m_shaderProgram = _shader;
	m_va = _va;
	m_mat = _mat;
	m_cam = GetApplication()->GetCamera();
}

void Renderer::OnInit(std::string _path, std::shared_ptr<ShaderProgram> _shader)
{
	//Convert the path to char*
	char* p = &_path[0];
	m_model = std::make_shared<Model>(p);
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
		if (m_va)
		{
			if (m_mat)
			{
				m_shaderProgram->SetUniform("in_Model", GetEntity()->GetTransform()->GetModelMatrix());
				
				//m_shaderProgram->SetUniform("irradianceMap", PBR_Material::GetIrradiance());
				glUseProgram(m_shaderProgram->GetId());
				glUniform1i(glGetUniformLocation(m_shaderProgram->GetId(), "irradianceMap"), 6);
				glActiveTexture(GL_TEXTURE6);
				glBindTexture(GL_TEXTURE_CUBE_MAP, PBR_Material::GetIrradiance());

				glUniform1i(glGetUniformLocation(m_shaderProgram->GetId(), "prefilterMap"), 7);
				glActiveTexture(GL_TEXTURE7);
				glBindTexture(GL_TEXTURE_CUBE_MAP, PBR_Material::GetPrefilter());

				glUniform1i(glGetUniformLocation(m_shaderProgram->GetId(), "brdfLUT"), 8);
				glActiveTexture(GL_TEXTURE8);
				glBindTexture(GL_TEXTURE_2D, PBR_Material::GetBRDF());

				int matBinary = 0;

				if (m_mat->GetAlbedo() != nullptr) {
					m_shaderProgram->SetUniform("in_Material.texture_diffuse1", m_mat->GetAlbedo());
					matBinary += 16;
				}
				else
					m_shaderProgram->SetUniform("in_Tex.albedo", m_mat->GetAlbedoValue());

				if (m_mat->GetNormal() != nullptr) {
					m_shaderProgram->SetUniform("in_Material.texture_normal1", m_mat->GetNormal());
					matBinary += 8;
				}
				else
					m_shaderProgram->SetUniform("in_Tex.normal", glm::vec3(0.0f, 0.0f, 1.0f));

				if (m_mat->GetMetallic() != nullptr) {
					m_shaderProgram->SetUniform("in_Material.texture_metallic1", m_mat->GetMetallic());
					matBinary += 4;
				}
				else
					m_shaderProgram->SetUniform("in_Tex.metallic", m_mat->GetMetallicValue());

				if (m_mat->GetRoughness() != nullptr) {
					m_shaderProgram->SetUniform("in_Material.texture_roughness1", m_mat->GetRoughness());
					matBinary += 2;
				}
				else
					m_shaderProgram->SetUniform("in_Tex.roughness", m_mat->GetRoughnessValue());

				if (m_mat->GetAO() != nullptr) {
					m_shaderProgram->SetUniform("in_Material.texture_ao1", m_mat->GetAO());
					matBinary += 1;
				}
				else
					m_shaderProgram->SetUniform("in_Tex.ao", 1.0f);
				m_shaderProgram->SetUniform("in_MatBinary", matBinary);
				//m_shaderProgram->SetUniform("in_Material.diffuse", m_mat->GetAlbedo());

				m_shaderProgram->Draw(m_va);
			}
			else
			{
				m_shaderProgram->SetUniform("in_Model", GetEntity()->GetTransform()->GetModelMatrix());
				m_shaderProgram->Draw(m_va);
			}
			
		}
		else
		{
			m_shaderProgram->SetUniform("in_Model", GetEntity()->GetTransform()->GetModelMatrix());
			//Using assimp
			m_model->Draw(m_shaderProgram);
		}

	}
	else
	{

	}
}
