#include "VanillaEngine.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

Application::Application() {}
Application::~Application()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
std::shared_ptr<Application> const Application::Init(int _w, int _h)
{
	std::shared_ptr<Application> app = std::make_shared<Application>();
	app->running = false;
	app->self = app;
	app->m_mainCamera->m_app = app;

	try { if (!glfwInit()) { throw Exception("GLFW not initialised"); } }
	catch (Exception& e) { std::cout << "VanillaEngine Exception: " << e.what() << std::endl; }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	app->m_window = glfwCreateWindow(_w, _h, "My Title", NULL, NULL);
	app->m_windowWidth = _w; app->m_windowHeight = _h;
	try {if (!app->m_window) { throw Exception("Window couldn't be created"); }}
	catch (Exception& e) { std::cout << "VanillaEngine Exception: " << e.what() << std::endl; }

	app->m_input->m_window = app->m_window;
	//glfwSetInputMode(app->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetErrorCallback(app->error_callback);
	glfwSetKeyCallback(app->m_window, app->m_input->KeyCallback);
	glfwSetCursorPosCallback(app->m_window, app->m_input->MouseCallback);

	glfwMakeContextCurrent(app->m_window);
	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwSwapInterval(1);

	try 
	{ 
		if (glewInit() != GLEW_OK) { throw Exception("GLEW couldn't be initialised"); } 
	}
	catch (Exception& e) { std::cout << "VanillaEngine Exception: " << e.what() << std::endl; }

	try
	{
		if (!app->m_resourceManager)
		{
			throw Exception("Resource manager not loaded");
		}
	}
	catch (Exception& e) { std::cout << "VanillaEngine Exception: " << e.what() << std::endl; }


	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	app->m_skybox->Init(app->self);
	SceneManager::Init(app->self);

	return app;
}
void Application::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		m_time->StartOfFrame();

		SceneManager::UpdateScene();

		try
		{
			m_mainCamera->UpdateMatrix(m_windowWidth, m_windowHeight);
		}
		catch (Exception& e)
		{
			std::cout << "myEngine Exception: " << e.what() << std::endl;
		}

		glClearColor(0.6f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_windowWidth, m_windowHeight);

		SceneManager::DrawScene();

		m_input->ResetDeltaMouseValues();
		glfwPollEvents();

		glfwSwapBuffers(m_window);

		m_time->CapFramerate(60.0f);
	}

}
void Application::Stop()
{
	running = false;
}

void Application::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

float Application::GetDeltaTime()
{
	return m_time->GetDeltaTime();
}