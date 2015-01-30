#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <SDL/SDL.h>
#include "ECSL/Framework/Multithreading/RuntimeInfo.h"
#include "Tables/ComponentTable.h"
#include "Tables/EntityTable.h"

#include <map>
#include <vector>

namespace ECSL
{
	class DECLSPEC DataManager
	{
	public:
		DataManager(unsigned int _entityCount, std::vector<unsigned int>* _componentTypeIds);
		~DataManager();

		void InitializeTables();

		unsigned int CreateNewEntity();
		void RemoveEntity(unsigned int _entityId);
		void CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId);
		void CreateComponentAndAddTo(unsigned int _componentTypeId, unsigned int _entityId);
		void RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId);
		void RemoveComponentFrom(unsigned int _componentTypeId, unsigned int _entityId);

		void UpdateEntityTable(const RuntimeInfo& _runtime);
		void RecycleEntityIds(const RuntimeInfo& _runtime);
		void CopyCurrentLists(const RuntimeInfo& _runtime);
		void ClearCopiedLists(const RuntimeInfo& _runtime);
		void DeleteComponentData(const RuntimeInfo& _runtime);

		inline unsigned int GetEntityCount() { return m_entityCount; }
		inline unsigned int GetComponentTypeCount() { return m_componentTypeCount; }
		inline EntityTable* GetEntityTable() { return m_entityTable; }
		inline ComponentTable* GetComponentTable(const std::string& _componentType) { return m_componentTables->at(ComponentTypeManager::GetInstance().GetTableId(_componentType)); }
		inline ComponentTable* GetComponentTable(unsigned int _componentTypeId) { return m_componentTables->at(_componentTypeId); }
		inline const std::vector<unsigned int>* GetChangedEntities() { return m_changedEntitiesCopy; }
		inline const std::map<unsigned int, std::vector<unsigned int>*>* GetComponentsToBeAdded() { return m_componentsToBeAddedCopy; }
		inline const std::map<unsigned int, std::vector<unsigned int>*>* GetComponentsToBeRemoved() { return m_componentsToBeRemovedCopy; }

		inline bool HasComponent(unsigned int _entityId, unsigned int _componentTypeId){ return m_entityTable->HasComponent(_entityId, _componentTypeId); }

		unsigned int GetMemoryAllocated();

	private:
		SDL_mutex* m_changedEntitiesMutex;
		SDL_mutex* m_entitiesToBeRemovedMutex;
		SDL_mutex* m_componentsToBeAddedMutex;
		SDL_mutex* m_componentsToBeRemovedMutex;
		SDL_mutex* m_componentDataToBeClearedMutex;
		unsigned int m_entityCount;
		unsigned int m_componentTypeCount;
		EntityTable* m_entityTable;
		std::vector<ComponentTable*>* m_componentTables;
		std::vector<unsigned int>* m_componentTypeIds;
		std::vector<unsigned int>* m_changedEntities;
		std::vector<unsigned int>* m_changedEntitiesCopy;
		std::vector<unsigned int>* m_entitiesToBeRemoved;
		std::vector<unsigned int>* m_entitiesToBeRemovedCopy;
		std::map<unsigned int, std::vector<unsigned int>*>* m_componentsToBeAdded;
		std::map<unsigned int, std::vector<unsigned int>*>* m_componentsToBeAddedCopy;
		std::map<unsigned int, std::vector<unsigned int>*>* m_componentsToBeRemoved;
		std::map<unsigned int, std::vector<unsigned int>*>* m_componentsToBeRemovedCopy;

		void Changed(unsigned int _entityId);
		void ToBeAdded(unsigned int _entityId, unsigned int _componentTypeId);
		void ToBeRemoved(unsigned int _entityId);
		void ToBeRemoved(unsigned int _entityId, unsigned int _componentTypeId);
	};
}

#endif