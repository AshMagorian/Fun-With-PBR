#pragma once
#include <VanillaEngine/VanillaEngine.h>

class SceneSwitcher : public Component
{
private:
	std::shared_ptr<Input> m_input;
public:

	void OnBegin();
	void OnTick();
};