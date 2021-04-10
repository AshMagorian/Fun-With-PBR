#pragma once

#include <vector>
#include <string>
#include <list>
#include <memory>

class Entity;

class DebugUIManager
{
	friend class Application;
private:

	static bool m_entityWindow;
	static std::string m_currentEntity;
	static std::weak_ptr<Application> m_app;

	static void Init(GLFWwindow* _window, std::weak_ptr<Application> _app);
	static void NewFrame();
	static void Tick(std::list<std::shared_ptr<Entity>> _entities, int _width, int _height);
	static void Display();
	static void End();
public:

};