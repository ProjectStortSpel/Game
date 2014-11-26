#ifndef WORLD_H
#define WORLD_H

#include <SDL/SDL.h>
#include <string>

#include "Systems/SystemWorkGroup.h"
#include "Systems/SystemManager.h"
#include "Tables/DataManager.h"


namespace ECSL
{
	class DECLSPEC World
	{
	private:
		DataManager* m_dataManager;
		SystemManager* m_systemManager;

	public:
		World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<int>* _componentTypeIds);
		~World();
	};
}

#endif