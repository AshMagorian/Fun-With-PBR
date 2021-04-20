#pragma once

#include <list>
#include <memory>

class ShaderProgram;
class Entity;
class Application;

class OutlineRenderer
{
private:
	std::list<std::shared_ptr<Entity>> m_list;
	std::weak_ptr<Application> m_app;
	std::shared_ptr<ShaderProgram> m_outlineShader;
public:
	void Init(std::weak_ptr<Application> _app);
	void AddToList(std::shared_ptr<Entity> _entity);
	void RenderOutlines();
	std::list<std::shared_ptr<Entity>> GetList() { return m_list; }
};