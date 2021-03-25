#include <VanillaEngine/VanillaEngine.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

bool DebugUIManager::m_entityWindow = false;
std::string DebugUIManager::m_currentEntity = "";

void DebugUIManager::Init(GLFWwindow* _window)
{
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

void DebugUIManager::Tick(std::list<std::shared_ptr<Entity>> _entities)
{
	ImGui::ShowDemoWindow();

	//Core window with scene selection, entities tab and skybox


	for (std::list<std::shared_ptr<Entity>>::iterator i = _entities.begin(); i != _entities.end(); ++i)
	{
		//Show entity names in the core window
	}
	
	if (m_entityWindow == true)
	{
		for (std::list<std::shared_ptr<Entity>>::iterator i = _entities.begin(); i != _entities.end(); ++i)
		{
			//Add to entities list on core?
			//if Window == true, display window with components
			if ((*i)->GetName() == m_currentEntity)
			{
				std::list<std::shared_ptr<Component>> components = (*i)->GetComponents();
				for (std::list<std::shared_ptr<Component>>::iterator j = components.begin(); j != components.end(); ++j)
				{
					(*j)->OnShowUI();
				}
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