#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>
#include <map>
#include <string>
#include <iostream>

#define IMPLEMENT_CLONE(TYPE) \
    std::shared_ptr<Component> Clone() const { return std::make_shared<TYPE>(*this); }
#define MAKE_PROTOTYPE(TYPE) \
    std::shared_ptr<Component> TYPE ## _myProtoype = \
      Component::AddPrototype(#TYPE, std::make_shared<TYPE>());

class Entity;
class Application;

class Component
{
	friend class Entity;
	friend class DebugUIManager;
private:
	std::weak_ptr<Entity> m_entity; // a reference to the entity which owns this component

	//static std::map<std::string, std::shared_ptr<Component>> protoTable;// Holds the names of each component type and it's corresponding prototype
	static std::map<std::string, std::shared_ptr<Component>>& protoTable() // Using Meyer's Singleton to make sure the prototype map is initialised before it is used
	{
		static std::map<std::string, std::shared_ptr<Component>> singleton;
		return singleton;
	}
	
	bool m_began; // Determines if onBegin should run
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
	/**
	*\brief Virtual funstion which clones the desired Component
	*/
	virtual std::shared_ptr<Component> Clone() const = 0;
public:
	/**
	*\brief Finds the appropriate prototype in the protoTable map and returns a clone of it
	*/
	static  std::shared_ptr<Component> MakeProduct(std::string type);
	/**
	*\brief Adds a prototype Component to th protoTable map
	*/
	static  std::shared_ptr<Component> AddPrototype(std::string type, std::shared_ptr<Component> p);

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