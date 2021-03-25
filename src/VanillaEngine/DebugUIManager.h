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


	static void Init(GLFWwindow* _window);
	static void NewFrame();
	static void Tick(std::list<std::shared_ptr<Entity>> _entities);
	static void Display();
	static void End();

public:

};