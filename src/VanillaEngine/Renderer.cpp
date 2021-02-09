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
	if (m_shaderProgram && m_mat && m_va)
	{
		m_shaderProgram->SetUniform("in_Model", GetEntity()->GetTransform()->GetModelMatrix());

		//m_shaderProgram->SetUniform("in_Material.albedo", m_mat->GetAlbedo());
		//m_shaderProgram->SetUniform("in_Material.normal", m_mat->GetNormal());
		//m_shaderProgram->SetUniform("in_Material.metallic", m_mat->GetMetallic());
		//m_shaderProgram->SetUniform("in_Material.roughness", m_mat->GetRoughness());
		//m_shaderProgram->SetUniform("in_Material.ao", m_mat->GetAO());

		//m_shaderProgram->Draw(m_va);
	}
	else
	{
		m_shaderProgram->SetUniform("in_Model", GetEntity()->GetTransform()->GetModelMatrix());
		//Using assimp
		m_model->Draw(m_shaderProgram);
	}
}
