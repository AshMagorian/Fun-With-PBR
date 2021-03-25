#include <VanillaEngine/VanillaEngine.h>

std::vector<std::shared_ptr<Scene>> SceneManager::m_scenes;
std::shared_ptr<Scene> SceneManager::m_currentScene;
std::shared_ptr<Scene> SceneManager::m_nextScene;
std::weak_ptr<Application> SceneManager::m_app;
bool SceneManager::m_isChangingScene = false;

void SceneManager::Init(std::weak_ptr<Application>_app)
{
	m_app = _app;
}

std::shared_ptr<Scene> SceneManager::CreateScene(std::string _name)
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	scene->sceneName = _name;
	scene->camera = scene->AddEntity("MainCamera");
	scene->lightManager = std::make_shared<Lights>();
	scene->lightManager->m_application = m_app;
	scene->self = scene;
	m_scenes.push_back(scene);
	return scene;
}

std::shared_ptr<Scene> SceneManager::GetScene(std::string _name)
{
	for (unsigned int i = 0; i < m_scenes.size(); ++i)
	{
		if (_name == m_scenes.at(i)->sceneName)
		{
			return m_scenes.at(i);
		}
	}
	std::cout << "Could not find scene! Scene: " << _name << " not found" << std::endl;
	return nullptr;
}

void SceneManager::SetCurrentScene(std::string _name)
{
	if (_name == m_currentScene->sceneName) { return; }
	for (unsigned int i = 0; i < m_scenes.size(); ++i)
	{
		if (_name == m_scenes.at(i)->sceneName)
		{
			m_nextScene = m_scenes.at(i);
			m_isChangingScene = true;
			return;
		}
	}
	std::cout << "Could not change scene! Scene: " << _name << " not found" << std::endl;
}

void SceneManager::SetStartupScene(std::string _name)
{
	for (unsigned int i = 0; i < m_scenes.size(); ++i)
	{
		if (_name == m_scenes.at(i)->sceneName)
		{
			m_currentScene = m_scenes.at(i);
			m_app.lock()->GetSkybox()->SetSkybox(m_currentScene->cubemapName);
			m_app.lock()->GetCamera()->SetCurrentCamera(m_currentScene->camera);
			m_nextScene = m_currentScene;
			m_isChangingScene = true;
			return;
		}
	}
	std::cout << "Could not set Startup scene! Scene: " << _name << " not found" << std::endl;
}

void SceneManager::UpdateScene()
{
	m_currentScene->lightManager->UpdateLightShaderValues();

	for (std::list<std::shared_ptr<Entity>>::iterator i = m_currentScene->entities.begin(); i != m_currentScene->entities.end(); ++i)
	{
		(*i)->Tick();
	}
}

void SceneManager::DrawScene()
{
	for (std::list<std::shared_ptr<Entity>>::iterator i = m_currentScene->entities.begin(); i != m_currentScene->entities.end(); ++i)
	{
		(*i)->Display();
	}
	m_app.lock()->GetSkybox()->DrawSkybox();

	//Check if the scene is to be changed / Sets the scene on the first frame
	if (m_isChangingScene == true)
	{
		ChangeScene();
	}
}

std::shared_ptr<Entity> Scene::AddEntity(std::string _name)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
	entity->m_self = entity;
	entity->m_scene = self;
	entity->m_application = SceneManager::m_app;
	entity->m_transform = entity->AddComponent<Transform>();

	std::list<std::shared_ptr<Entity>>::iterator i;
	int count = 0;
	std::string name = _name;
	do
	{ 
		for (i = entities.begin(); i != entities.end(); ++i)
		{
			if ((*i)->m_name == _name)
			{
				count++;
				name = _name + std::to_string(count);
				break;
			}
		}

	} while (i != entities.end());

	entity->m_name = name;
	entities.push_back(entity);
	return entity;
}

void SceneManager::ChangeScene()
{
	m_currentScene = m_nextScene;
	m_app.lock()->GetSkybox()->SetSkybox(m_currentScene->cubemapName);
	m_app.lock()->GetCamera()->SetCurrentCamera(m_currentScene->camera);
	for (std::list<std::shared_ptr<Entity>>::iterator i = m_currentScene->entities.begin(); i != m_currentScene->entities.end(); ++i)
	{
		(*i)->OnSceneBegin();
	}
	m_isChangingScene = false;
}