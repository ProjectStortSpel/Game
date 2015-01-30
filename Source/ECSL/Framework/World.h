#ifndef WORLD_H
#define WORLD_H

#include <SDL/SDL.h>
#include <string>

#include "Simulation.h"
#include "Components/DataManager.h"
#include "Multithreading/Scheduler.h"
#include "Systems/SystemWorkGroup.h"
#include "Systems/SystemManager.h"
#include "Templates/EntityTemplate.h"
#include "Systems/Messaging/MessageManager.h"

namespace ECSL
{
	class DECLSPEC World
	{
	private:
		Simulation* m_simulation;
		MessageManager* m_messageManager;
		DataManager* m_dataManager;

		SystemManager* m_systemManager;

	public:
		World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<unsigned int>* _componentTypeIds);
		~World();

		void Update(float _dt);

		unsigned int CreateNewEntity();
		unsigned int CreateNewEntity(const std::string& _templateName);

		void CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId);
		void RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId);

		DataLocation GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName);
		DataLocation GetComponent(unsigned int _entityId, const std::string& _componentType, const int _index);
		DataLocation GetComponent(unsigned int _entityId, const unsigned int _componentType, const int _index);

		void SetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName, void* _data, bool _notifyNetwork = true);

		void KillEntity(unsigned int _entityId);
		
		void GetEntityComponents(std::vector<unsigned int>& _out, unsigned int _entityId){ m_dataManager->GetEntityTable()->GetEntityComponents(_out, _entityId); }

		unsigned int GetMemoryUsage();
		
		void ComponentHasChanged(unsigned int _entityId, std::string _componentType, bool _notifyNetwork = true);
		void ComponentHasChanged(unsigned int _entityId, unsigned int _componentTypeId, bool _notifyNetwork = true);

		bool HasComponent(unsigned int _entityId, std::string _componentType);
		bool HasComponent(unsigned int _entityId, unsigned int _componentTypeId);
	};
}

#endif