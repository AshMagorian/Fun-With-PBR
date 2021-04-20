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
		//Get the model matrix and scale it by 1.1 before binding it to the shader
		tmp = (*it)->GetTransform()->GetModelMatrix();
		tmp = glm::scale(tmp, glm::vec3(1.02f, 1.02f, 1.02f));
		m_outlineShader->SetUniform("in_Model", tmp);

		//draw function
		if ((*it)->GetComponent<Renderer>()->m_va)
		{
			m_outlineShader->Draw((*it)->GetComponent<Renderer>()->m_va);
		}
	}
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
	m_list.clear();
}