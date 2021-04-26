#include <VanillaEngine/VanillaEngine.h>
#include "SaveManager.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

bool DebugUIManager::m_entityWindow = false;
std::shared_ptr<Entity> DebugUIManager::m_currentEntity = nullptr;
std::shared_ptr<SceneManager> DebugUIManager::m_sceneManager = nullptr;
std::weak_ptr<Application>DebugUIManager::m_app;
bool DebugUIManager::m_newScene = true;
bool DebugUIManager::m_resetEntityWindowSize = false;

void DebugUIManager::Init(GLFWwindow* _window, std::weak_ptr<Application> _app)
{
	m_app = _app;
	m_sceneManager = m_app.lock()->GetSceneManager();
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
	std::shared_ptr<Scene> currentScene = m_sceneManager->GetCurrentScene();
	//ImGui::ShowDemoWindow();
	if (m_newScene)
		ImGui::SetNextWindowSize(ImVec2(300.0f, 600.0f));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	std::string coreTitle = "Core (" + currentScene->sceneName + ")";
	ImGui::Begin(&coreTitle[0], NULL, ImGuiWindowFlags_MenuBar);
	m_newScene = false;
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::BeginMenu("Change Scene")) 
			{
				for (size_t i = 0; i < m_sceneManager->m_scenes.size(); ++i)
				{
					std::string tmp = m_sceneManager->m_scenes.at(i)->sceneName;
					if (ImGui::MenuItem(&tmp[0]))
					{
						m_sceneManager->SetCurrentScene(tmp);
						m_newScene = true;
					}
				}

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save Scene"))
			{
				m_app.lock()->GetSaveManager()->SaveScene();
			}
			if (ImGui::BeginMenu("Load Scene"))
			{
				std::vector<std::string> names = m_app.lock()->GetSaveManager()->GetScenes();
				for (size_t i = 0; i < names.size(); ++i)
				{
					std::string tmp = names.at(i);
					if (ImGui::MenuItem(&tmp[0]))
					{
						m_app.lock()->GetSaveManager()->LoadScene(tmp);
						m_newScene = true;
					}
				}
				ImGui::EndMenu();
			}

			
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//Core window with scene selection, entities tab and skybox

	//bool resetWindow = false;
	if (ImGui::CollapsingHeader("Entities"))
	{
		for (std::list<std::shared_ptr<Entity>>::iterator it = _entities.begin(); it != _entities.end(); ++it)
		{
			std::string tmp = (*it)->GetName();
			char* label = &tmp[0];
			if (ImGui::Selectable(label, m_currentEntity == (*it) && m_entityWindow == true))
			{
				m_currentEntity = (*it);
				m_entityWindow = true;
				m_resetEntityWindowSize = true;
			}
		}
		if (ImGui::Button("Add Entity"))
			ImGui::OpenPopup("entity_popup");
		if (ImGui::BeginPopup("entity_popup"))
		{
			ImGui::Text("New Entity Name");
			ImGui::Separator();
			static char entityName[64] = ""; ImGui::InputText("", entityName, 64);
			if (ImGui::Button("Add") && entityName[0] != NULL)
			{
				currentScene->AddEntity(entityName);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

	}
	// Directional light
	if (ImGui::CollapsingHeader("Directional Light"))
	{
		ImGui::Text("    x        y         z");
		ImGui::DragFloat3("Direction", &(currentScene->lightManager->GetDirectionalLight()->direction.x), 0.005f);
		ImGui::ColorPicker3("Colour", &(currentScene->lightManager->GetDirectionalLight()->colour.x));
		ImGui::DragFloat("Intensity", &(currentScene->lightManager->GetDirectionalLight()->intensity), 0.05f);
	}
	// Skybox selector
	if (ImGui::CollapsingHeader("Skybox"))
	{
		const int numCubeMaps = Skybox::m_cubemaps.size();
		std::vector<char*> maps;
		for (int i = 0; i < numCubeMaps; ++i)
			maps.push_back(&Skybox::m_cubemaps.at(i).name[0]);
		const char* mapsArr[100];
		std::copy(maps.begin(), maps.end(), mapsArr);
		static int item_current = 0;
		ImGui::Combo("skybox", &item_current, mapsArr, maps.size());
		if (ImGui::Button("Apply"))
		{
			currentScene->cubemapName = maps.at(item_current);
			m_app.lock()->GetSkybox()->SetSkybox(maps.at(item_current));
		}
		GLuint id = Skybox::m_cubemaps[item_current].preview_id;

		ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
		ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
		
		ImGui::Image((void*)(intptr_t)id, ImVec2(200, 200), uv_min, uv_max, tint_col, border_col);
		
	}

	ImGui::End();
	
	if (m_entityWindow == true)
	{
		bool deleteFlag = false;
		for (std::list<std::shared_ptr<Entity>>::iterator i = _entities.begin(); i != _entities.end(); ++i)
		{
			//Add to entities list on core?
			//if Window == true, display window with components
			if ((*i) == m_currentEntity)
			{
				ImVec2 size{ 300.0f, 600.0f };
				ImVec2 pos{(float)_width - size.x, 0.0f};
				if (m_resetEntityWindowSize)
				{
					ImGui::SetNextWindowSize(size);
					ImGui::SetNextWindowPos(pos);
					m_resetEntityWindowSize = false;
				}

				ImGui::Begin((*i)->GetName().c_str(), &m_entityWindow, ImGuiWindowFlags_MenuBar);
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Menu"))
					{
						if (ImGui::MenuItem("Delete Entity"))
							deleteFlag = true;
						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();
				}


				std::list<std::shared_ptr<Component>> components = (*i)->GetComponents();
				std::shared_ptr<Component> componentToDelete = nullptr;
				for (std::list<std::shared_ptr<Component>>::iterator j = components.begin(); j != components.end(); ++j)
				{
					std::string componentName = typeid(*(*j)).name();
					ImGui::PushID(componentName.c_str());
					componentName = componentName.substr(componentName.find_first_of(" \t")+1);
					if (ImGui::CollapsingHeader(componentName.c_str()))
					{
						(*j)->OnShowUI();
						if (ImGui::SmallButton("Remove Component"))
							componentToDelete = (*j);
					}
					if(componentName == "Renderer")
						(*i)->GetComponent<Renderer>()->DrawOutline();

					ImGui::Separator();
					ImGui::PopID();
				}
				if (componentToDelete!= nullptr)
					(*i)->RemoveComponent(componentToDelete);

				if (ImGui::Button("Add Component"))
					ImGui::OpenPopup("my_select_popup");
				if (ImGui::BeginPopup("my_select_popup"))
				{
					ImGui::Text("Components");
					ImGui::Separator();
					
					for (auto it = Component::protoTable().cbegin(); it != Component::protoTable().cend(); ++it)
					{
						if (ImGui::Selectable((it->first).c_str()))
						{
							(*i)->AddComponentPrototype((it->first));
						}
					}
					ImGui::EndPopup();
				}


				if (ImGui::Button("Rename Entity"))
					ImGui::OpenPopup("rename_popup");
				if (ImGui::BeginPopup("rename_popup"))
				{
					ImGui::Text("New Name");
					ImGui::Separator();
					static char newName[64] = ""; ImGui::InputText("", newName, 64);
					if (ImGui::Button("confirm") && newName[0] != NULL)
					{
						currentScene->NameEntity(newName, (*i));
						m_resetEntityWindowSize = true;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				ImGui::End();
			}
		}
		if (deleteFlag == true)
			currentScene->entities.remove(m_currentEntity);
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