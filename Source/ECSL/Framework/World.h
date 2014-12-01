#ifndef WORLD_H
#define WORLD_H

#include <SDL/SDL.h>
#include <string>

#include "Components/DataManager.h"
#include "Systems/SystemWorkGroup.h"
#include "Systems/SystemManager.h"
#include "Simulation.h"

namespace ECSL
{
	class DECLSPEC World
	{
	private:
		Simulation* m_simulation;
		DataManager* m_dataManager;
		SystemManager* m_systemManager;
		

	public:
		World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<unsigned int>* _componentTypeIds);
		~World();

		void Update(float _dt);

		inline unsigned int CreateNewEntity()
		{
			return m_dataManager->CreateNewEntity();
		}
		inline void CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId)
		{
			m_dataManager->CreateComponentAndAddTo(_componentType, _entityId);
		}
		inline void KillEntity(unsigned int _entityId)
		{
			m_dataManager->KillEntity(_entityId);
		}
	};
}

#endif