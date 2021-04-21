#include "OutlineRenderer.h"
#include "VanillaEngine.h"
void OutlineRenderer::Init(std::weak_ptr<Application> _app)
{
	m_outlineShader = std::make_shared<ShaderProgram>("../src/resources/shaders/singleColor.vert", "../src/resources/shaders/singleColor.frag");
	m_app = _app;
}
void OutlineRenderer::AddToList(std::shared_ptr<Entity> _entity)
{
	m_list.push_back(_entity);
}
void OutlineRenderer::RenderOutlines()
{
	//Set view and projection matrix
	m_outlineShader->SetUniform("in_Projection", m_app.lock()->GetCamera()->GetProjectionMatrix());
	m_outlineShader->SetUniform("in_View", m_app.lock()->GetCamera()->GetViewMatrix());

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00); // disable writing to the stencil buffer
	glDisable(GL_DEPTH_TEST);
	
	glm::mat4 tmp;
	for (std::list<std::shared_ptr<Entity>>::iterator it = m_list.begin(); it != m_list.end(); ++it)
	{
		glm::vec3 inverseScale = 1.0f / (*it)->GetTransform()->GetScale();
		//draw function
		if ((*it)->GetComponent<Renderer>()->m_assimpModel)
		{

			m_outlineShader->SetUniform("inverseScale", inverseScale);
			m_outlineShader->SetUniform("in_Model", (*it)->GetTransform()->GetModelMatrix());
			m_outlineShader->SetUniform("assimpMesh", 1);
			(*it)->GetComponent<Renderer>()->m_assimpModel->DrawOutline(m_outlineShader);
		}
		else if ((*it)->GetComponent<Renderer>()->m_va)
		{
			tmp = (*it)->GetTransform()->GetModelMatrix();

			tmp = glm::scale(tmp, glm::vec3(1.0f, 1.0f, 1.0f) +(0.02f * inverseScale));
			m_outlineShader->SetUniform("in_Model", tmp);
			m_outlineShader->SetUniform("assimpMesh", 0);
			m_outlineShader->Draw((*it)->GetComponent<Renderer>()->m_va);
		}
	}
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
	m_list.clear();
}