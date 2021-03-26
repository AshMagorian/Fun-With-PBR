#include <VanillaEngine/VanillaEngine.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

bool DebugUIManager::m_entityWindow = false;
std::string DebugUIManager::m_currentEntity = "";
std::weak_ptr<Application>DebugUIManager::m_app;

void DebugUIManager::Init(GLFWwindow* _window, std::weak_ptr<Application> _app)
{
	m_app = _app;
	// Setup ImGui 
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
	ImGui::StyleColorsDark();
}

void DebugUIManager::NewFrame()
{
	//imgui new Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void DebugUIManager::Tick(std::list<std::shared_ptr<Entity>> _entities, int _width, int _height)
{
	ImGui::ShowDemoWindow();
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Core", NULL, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::BeginMenu("Change Scene")) 
			{
				for (size_t i = 0; i < SceneManager::m_scenes.size(); ++i)
				{
					std::string tmp = SceneManager::m_scenes.at(i)->sceneName;
					if (ImGui::MenuItem(&tmp[0]))
					{
						SceneManager::SetCurrentScene(tmp);
					}
				}

				ImGui::EndMenu();
			}
			
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//Core window with scene selection, entities tab and skybox

	bool resetWindow = false;
	if (ImGui::CollapsingHeader("Entities"))
	{
		for (std::list<std::shared_ptr<Entity>>::iterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			std::string tmp = (*it)->GetName();
			char* label = &tmp[0];
			if (ImGui::Selectable(label, m_currentEntity == (*it)->GetName() && m_entityWindow == true))
			{
				m_currentEntity = (*it)->GetName();
				m_entityWindow = true;
				resetWindow = true;
			}
		}
	}
	// Directional light
	if (ImGui::CollapsingHeader("Directional Light"))
	{
		ImGui::Text("    x        y         z");
		ImGui::DragFloat3("Direction", &(SceneManager::GetCurrentScene()->lightManager->GetDirectionalLight()->direction.x), 0.005f);
		ImGui::ColorPicker3("Colour", &(SceneManager::GetCurrentScene()->lightManager->GetDirectionalLight()->colour.x));
		ImGui::DragFloat("Intensity", &(SceneManager::GetCurrentScene()->lightManager->GetDirectionalLight()->intensity), 0.05);
	}
	if (ImGui::CollapsingHeader("Skybox"))
	{
		// Skybox selector
		const int numCubeMaps = Skybox::m_cubemaps.size();
		std::vector<char*> maps;
		for (size_t i = 0; i < numCubeMaps; ++i)
			maps.push_back(&Skybox::m_cubemaps.at(i).name[0]);
		const char* mapsArr[100];
		std::copy(maps.begin(), maps.end(), mapsArr);
		static int item_current = 0;
		ImGui::Combo("skybox", &item_current, mapsArr, maps.size());
		if (ImGui::Button("Apply"))
		{
			SceneManager::GetCurrentScene()->cubemapName = maps.at(item_current);
			m_app.lock()->GetSkybox()->SetSkybox(maps.at(item_current));
		}
	}
	ImGui::End();
	
	if (m_entityWindow == true)
	{
		for (std::list<std::shared_ptr<Entity>>::iterator i = _entities.begin(); i != _entities.end(); ++i)
		{
			//Add to entities list on core?
			//if Window == true, display window with components
			if ((*i)->GetName() == m_currentEntity)
			{
				ImVec2 size{ 300.0f, 600.0f };
				ImVec2 pos{(float)_width - size.x, 0.0f};
				if (resetWindow)
				{
					ImGui::SetNextWindowSize(size);
					ImGui::SetNextWindowPos(pos);
				}

				ImGui::Begin((*i)->GetName().c_str(), &m_entityWindow);

				std::list<std::shared_ptr<Component>> components = (*i)->GetComponents();
				for (std::list<std::shared_ptr<Component>>::iterator j = components.begin(); j != components.end(); ++j)
				{
					(*j)->OnShowUI();
				}
				ImGui::End();
				resetWindow = false;
			}
		}
	}
}

void DebugUIManager::Display()
{
	//Render imgui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugUIManager::End()
{
	//Shutdown imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}