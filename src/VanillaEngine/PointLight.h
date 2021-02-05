#ifndef POINT_LIGHT
#define POINT_LIGHT

#include "Lights.h"
#include "Component.h"
#include "Entity.h"
#include "Application.h"

class PointLight : public Component
{
	friend class Lights;
private:
	glm::vec3 m_position;
	glm::vec3 m_colour;

public:
	void OnInit()
	{
		m_position = GetEntity()->GetTransform()->GetPos();
		m_colour = glm::vec3(20.0f, 20.0f, 20.0f);
		GetApplication()->GetLightManager()->AddPointLight(GetEntity());
	}

	void OnTick()
	{
		m_position = GetEntity()->GetTransform()->GetPos();
	}
	void SetColour(glm::vec3 _colour) { m_colour = _colour; }
};
#endif