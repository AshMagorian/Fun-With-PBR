#pragma once

#include <vector>
#include <list>
#include <memory>

class Entity;

class DebugUIManager
{
	friend class Application;
private:

	static void Tick(std::list<std::shared_ptr<Entity>> _entities);

public:

};