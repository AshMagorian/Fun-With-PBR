#include <VanillaEngine/VanillaEngine.h>

void Tick(std::list<std::shared_ptr<Entity>> _entities)
{
	//Core window with scene selection, entities tab and skybox

	for (std::list<std::shared_ptr<Entity>>::iterator i = _entities.begin(); i != _entities.end(); ++i)
	{
		//Add to entities list on core?
		//if Window == true, display window with components
		(*i);
		for (std::list<std::shared_ptr<Component>>::iterator j = (*i)->GetComponents().begin(); j != (*i)->GetComponents().end(); ++j)
		{
			(*j)->OnShowUI();
		}
	}
}