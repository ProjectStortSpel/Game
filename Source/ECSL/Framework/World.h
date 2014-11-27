#ifndef WORLD_H
#define WORLD_H

#include <SDL/SDL.h>
#include <string>

#include "Components/DataManager.h"
#include "Systems/SystemWorkGroup.h"
#include "Systems/SystemManager.h"

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

		inline unsigned int CreateNewEntity();
		inline void CreateComponentAndAddTo(std::string& _componentType, unsigned int _id);
	};
}

#endif