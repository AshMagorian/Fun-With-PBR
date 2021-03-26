#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>

class Lights;
class Entity;

struct Scene
{
	std::string sceneName;
	std::shared_ptr<Entity> camera;
	std::list<std::shared_ptr<Entity>> entities;
	std::shared_ptr<Lights> lightManager;
	std::string cubemapName;
	std::weak_ptr<Scene> self;

	std::shared_ptr<Entity> AddEntity(std::string _name);
};

class SceneManager
{
	friend class Application;
	friend struct Scene;
	friend class DebugUIManager;
private:
	static std::vector<std::shared_ptr<Scene>> m_scenes;

	static std::shared_ptr<Scene> m_currentScene;
	static std::shared_ptr<Scene> m_nextScene;
	static bool m_isChangingScene;


	static void Init(std::weak_ptr<Application>_app);
	static void UpdateScene();
	static void DrawScene();
	static void ChangeScene();

public:

	static std::shared_ptr<Scene> CreateScene(std::string _name);
	static std::shared_ptr<Scene> GetCurrentScene() { return m_currentScene; }
	static std::shared_ptr<Scene> GetScene(std::string _name);
	static void SetStartupScene(std::string _name);
	static void SetCurrentScene(std::string _name);
protected:
	static std::weak_ptr<Application> m_app;
};