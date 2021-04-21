#include "VanillaEngine.h"
#include "AssimpModel.h"

MAKE_PROTOTYPE(Renderer)

Renderer::Renderer() {}
Renderer::~Renderer() 
{
	if (m_drawOutline == true)
		GetApplication()->GetOutlineRenderer()->m_list.remove(GetEntity());
}

void Renderer::OnInit()
{
	m_shaderProgram = GetApplication()->GetResourceManager()->LoadFromResources<ShaderProgram>("pbr_shader");
	m_va = GetApplication()->GetResourceManager()->LoadFromResources<VertexArray>("sphere");
	m_pbrMat = std::make_shared<PBR_Material>();
	m_cam = GetApplication()->GetCamera();
}
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
	m_assimpModel = std::make_shared<AssimpModel>(p);
	m_shaderProgram = _shader;
	m_cam = GetApplication()->GetCamera();
}
void Renderer::OnInit(std::shared_ptr<AssimpModel> _assimp, std::shared_ptr<ShaderProgram> _shader)
{
	m_assimpModel =_assimp;
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
		if (m_assimpModel)
		{
			//Render using assimp
			BindIBLMaps();
			if (m_drawOutline)
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
				glStencilMask(0xFF); // enable writing to the stencil buffer
				m_assimpModel->Draw(m_shaderProgram);
				glStencilFunc(GL_ALWAYS, 0, 0xFF);
				glStencilMask(0x00);
				m_drawOutline = false;
			}
			else
			 m_assimpModel->Draw(m_shaderProgram);
		}
		else if (m_va)
		{
			if (m_pbrMat)
				BindPBRValues();
			if (m_drawOutline)
			{
				glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
				glStencilMask(0xFF); // enable writing to the stencil buffer
				m_shaderProgram->Draw(m_va);
				glStencilFunc(GL_ALWAYS, 0, 0xFF);
				glStencilMask(0x00);
				m_drawOutline = false;
			}
			else
				m_shaderProgram->Draw(m_va);
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

void Renderer::DrawOutline()
{ 
	m_drawOutline = true; 
	GetApplication()->GetOutlineRenderer()->AddToList(GetEntity()); 
}


void Renderer::OnShowUI()
{
	if (m_shaderProgram)
	{
		ImGui::Text(("Shader: " + m_shaderProgram->GetName()).c_str());
	}
	ImGui::SameLine(150); 
	if (ImGui::SmallButton("Select Shader"))
		ImGui::OpenPopup("shader_popup");
	if (ImGui::BeginPopup("shader_popup"))
	{
		ImGui::Text("Shaders");
		ImGui::Separator();

		std::list<std::shared_ptr<ShaderProgram>> list;
		GetApplication()->GetResourceManager()->GetAll(&list);
		for (std::list<std::shared_ptr<ShaderProgram>>::iterator i = list.begin(); i != list.end(); ++i)
		{
			if (ImGui::Selectable((*i)->GetName().c_str()))
				m_shaderProgram = (*i);
		}
		ImGui::EndPopup();
	}

	if (m_assimpModel)
	{
		ImGui::Text(("Assimp Mesh: " + m_assimpModel->GetName()).c_str());
	}
	else if (m_va)
	{
		ImGui::Text(("Mesh: " + m_va->GetName()).c_str());

	}
	ImGui::SameLine(150);
	if (ImGui::SmallButton("Select Mesh"))
		ImGui::OpenPopup("mesh_popup");
	if (ImGui::BeginPopup("mesh_popup"))
	{
		ImGui::Text("Meshes");
		ImGui::Separator();

		std::list<std::shared_ptr<VertexArray>> list;
		GetApplication()->GetResourceManager()->GetAll(&list);
		for (std::list<std::shared_ptr<VertexArray>>::iterator i = list.begin(); i != list.end(); ++i)
		{
			if (ImGui::Selectable((*i)->GetName().c_str()))
			{
				m_assimpModel = nullptr;
				m_va = std::make_shared<VertexArray>(*(*i));
			}
		}
		ImGui::Separator();
		ImGui::Text("Assimp Meshes");
		ImGui::Separator();

		std::list<std::shared_ptr<AssimpModel>> list2;
		GetApplication()->GetResourceManager()->GetAll(&list2);
		for (std::list<std::shared_ptr<AssimpModel>>::iterator i = list2.begin(); i != list2.end(); ++i)
		{
			if (ImGui::Selectable((*i)->GetName().c_str()))
			{
				m_va = nullptr;
				m_assimpModel = std::make_shared<AssimpModel>(*(*i));
			}
		}
		ImGui::EndPopup();
	}

	if (m_pbrMat && !m_assimpModel)
	{
		if (ImGui::TreeNode("PBR Material"))
		{
			ImGui::DragFloat("TexCoord Scale", &m_texCoordScale, 0.01f);
			m_pbrMat->ShowUI();
			if (ImGui::TreeNode("Displacement"))
			{
				if (m_pbrMat->GetDisplacement() != nullptr)
				{
					ImGui::Text(("displacement texture: " + m_pbrMat->GetDisplacement()->GetPath()).c_str());
					ImGui::Image((void*)(intptr_t)m_pbrMat->GetDisplacement()->getId(), ImVec2(100, 100));
					if (ImGui::SmallButton("Remove texture##disp"))
						m_pbrMat->SetDisplacementTex(nullptr);

					if (ImGui::RadioButton("Parallax clipBorders", m_parallax_clipBorders))
						m_parallax_clipBorders = !m_parallax_clipBorders;
					ImGui::DragFloat("Parallax height", &m_parallax_height, 0.001f);
					ImGui::InputInt("Parallax min layers", &m_parallax_minLayers);
					ImGui::InputInt("Parallax max layers", &m_parallax_maxLayers);
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		else
		{
			ImGui::SameLine(150);
			if (ImGui::SmallButton("Select PBR Material##b"))
				ImGui::OpenPopup("pbr_popup");
		}
	}
	if (ImGui::BeginPopup("pbr_popup"))
	{
		ImGui::Text("PBR Materials");
		ImGui::Separator();

		std::list<std::shared_ptr<PBR_Material>> list;
		GetApplication()->GetResourceManager()->GetAll(&list);
		for (std::list<std::shared_ptr<PBR_Material>>::iterator i = list.begin(); i != list.end(); ++i)
		{
			if (ImGui::Selectable((*i)->GetName().c_str()))
				m_pbrMat = GetApplication()->GetResourceManager()->LoadFromResources<PBR_Material>((*i)->GetName());
		}
		ImGui::EndPopup();
	}
}