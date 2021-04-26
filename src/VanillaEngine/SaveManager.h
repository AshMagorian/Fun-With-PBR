#pragma once
#include <json.h>
#include <iostream>
#include <fstream>

class Application;
class SceneManager;

class SaveManager
{
	friend class Application;
private:
	std::weak_ptr<Application> m_app;
	std::shared_ptr<SceneManager> m_sceneManager;

	void Init(std::weak_ptr<Application> _app);

	void SetVec3(Json::Value& _val, glm::vec3 _vec);
	glm::vec3 GetVec3(Json::Value& _val);

public:
	void SaveScene();
	void LoadScene(std::string _sceneName);
	std::vector<std::string> GetScenes();
};