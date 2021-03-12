#include "SceneSwitcher.h"

void SceneSwitcher::OnBegin()
{
	m_input = GetApplication()->GetInput();
}

void SceneSwitcher::OnTick()
{
	if (m_input->IsKeyPressed('1'))
	{
		SceneManager::SetCurrentScene("Scene1");
	}
	if (m_input->IsKeyPressed('2'))
	{
		SceneManager::SetCurrentScene("Scene2");
	}
}