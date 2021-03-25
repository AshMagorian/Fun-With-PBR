#pragma once
#include <VanillaEngine/VanillaEngine.h>

class FirstPersonCameraControls : public Component
{
private:
	std::shared_ptr<Transform> m_Transform; ///< The transform component 

	glm::vec3 m_startPos;
	glm::vec3 m_startRot;

	float m_movementSpeed = 3.0f; ///< The speed in which the player moves forward

public:
	void OnBegin();
	void OnSceneBegin();
	void OnTick();
};