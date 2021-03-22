#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <string>
#include <iostream>

class Entity;
class Application;

class Component
{
	friend class Entity;
	friend class DebugUIManager;
private:
	std::weak_ptr<Entity> m_entity; ///< a reference to the entity which owns this component
	bool m_began; ///< Determines if onBegin should run
	/**
	*\brief Runs on startup
	*/
	virtual void OnInit() {}
	/**
	*\brief Runs on the first frame of startup
	*/
	virtual void OnBegin() {}
	/**
	*\brief Runs on the first frame when the scene starts
	*/
	virtual void OnSceneBegin() {}
	/**
	*\brief Runs every frame
	*/
	virtual void OnTick() {}
	/**
	*\brief Calls the rendering functions (Only useful for the Render class)
	*/
	virtual void OnDisplay() {}

public:
	Component();
	virtual ~Component();

	/**
	*\brief Gives instructions on how to display debug UI elements
	*/
	virtual void OnShowUI() {}

	std::shared_ptr<Entity> GetEntity();
	std::shared_ptr<Application> GetApplication();
};
#endif