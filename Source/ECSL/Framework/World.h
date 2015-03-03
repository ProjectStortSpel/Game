#ifndef WORLD_H
#define WORLD_H

#include <SDL/SDL.h>
#include <string>

#include "Simulation/Simulation.h"
#include "Components/DataManager.h"
#include "Multithreading/Scheduler.h"
#include "Systems/SystemWorkGroup.h"
#include "Systems/SystemManager.h"
#include "Templates/EntityTemplate.h"
#include "Systems/Messaging/MessageManager.h"
#include "Logger/DataLogger.h"

namespace ECSL
{
	class DECLSPEC World
	{
	private:
		Simulation* m_simulation;
		MessageManager* m_messageManager;
		DataManager* m_dataManager;
		SystemManager* m_systemManager;
		DataLogger* m_dataLogger;

		

	public:
		World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<unsigned int>* _componentTypeIds);
		~World();
		
		void PostInitializeSystems();

		void Update(float _dt);

		unsigned int CreateNewEntity();
		unsigned int CreateNewEntity(const std::string& _templateName);

		void KillEntity(unsigned int _entityId);

		void CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId);
		void RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId);

		DataLocation GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName);
		DataLocation GetComponent(unsigned int _entityId, const std::string& _componentType, const int _index);
		DataLocation GetComponent(unsigned int _entityId, const unsigned int _componentType, const int _index);
		void GetComponents(std::vector<DataLocation>& _out, const std::vector<unsigned int>* _entityIds, const std::string& _componentType, const std::string& _variableName);
		void GetComponents(std::vector<DataLocation>& _out, const std::vector<unsigned int>* _entityIds, const std::string& _componentType, unsigned int _index);
		void GetComponents(std::vector<DataLocation>& _out, const std::vector<unsigned int>* _entityIds, unsigned int _componentTypeId, unsigned int _index);
		void GetComponents(std::vector<DataLocation>& _out, const unsigned int* _entities, unsigned int _entityCount, const std::string& _componentType, const std::string& _variableName);
		void GetComponents(std::vector<DataLocation>& _out, const unsigned int* _entities, unsigned int _entityCount, const std::string& _componentType, unsigned int _index);
		void GetComponents(std::vector<DataLocation>& _out, const unsigned int* _entities, unsigned int _entityCount, unsigned int _componentTypeId, unsigned int _index);
		void SetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName, void* _data, bool _notifyNetwork = true);
		bool HasComponent(unsigned int _entityId, const std::string& _componentType);
		bool HasComponent(unsigned int _entityId, unsigned int _componentTypeId);
		
		void GetEntityComponents(std::vector<unsigned int>& _out, unsigned int _entityId);
		
		void ComponentHasChanged(unsigned int _entityId, std::string _componentType, bool _notifyNetwork = true);
		void ComponentHasChanged(unsigned int _entityId, unsigned int _componentTypeId, bool _notifyNetwork = true);

		unsigned int GetMemoryUsage();
		
		void LogWorldData();

		//String?
		void SetString(unsigned int _eId, const std::string& _componentType, const unsigned int _index, const char* _value);
		void SetString(unsigned int _eId, unsigned int _componentTypeId, const unsigned int _index, const char* _value);

		std::string GetString(unsigned int _eId, const std::string& _componentType, const unsigned int _index);
		std::string GetString(unsigned int _eId, unsigned int _componentTypeId, const unsigned int _index);

		
	};
}

#endif