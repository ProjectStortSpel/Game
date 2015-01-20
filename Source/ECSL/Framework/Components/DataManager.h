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
	class EntityComponents
	{
	public:
		unsigned int m_entityId;
		std::vector<unsigned int>* m_componentTypeIds;
	private:
		EntityComponents(unsigned int _entityId) : m_entityId(_entityId) { m_componentTypeIds = new std::vector<unsigned int>(); }
		~EntityComponents() { delete(m_componentTypeIds); }

		void AddComponentTypeId(unsigned int _componentTypeId) { m_componentTypeIds->push_back(_componentTypeId); }
		void ClearComponentTypes() { m_componentTypeIds->clear(); }
	};

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

		/* Update the entity table for added and removed components to entities */
		void UpdateEntityTable(const RuntimeInfo& _runtime);

		/* Clear dead entity data from entity table */
		void ClearDeadEntities(const RuntimeInfo& _runtime);

		void RecycleEntityIds(const RuntimeInfo& _runtime);

		void ClearComponentData(const RuntimeInfo& _runtime);

		void ClearComponentChangeLists(const RuntimeInfo& _runtime);

		void ClearDeadEntitiesChangeLists(const RuntimeInfo& _runtime);

		inline unsigned int GetEntityCount() { return m_entityCount; }
		inline EntityTable* GetEntityTable() { return m_entityTable; }
		inline ComponentTable* GetComponentTable(const std::string& _componentType) { return m_componentTables->at(ComponentTypeManager::GetInstance().GetTableId(_componentType)); }
		inline ComponentTable* GetComponentTable(unsigned int _componentTypeId) { return m_componentTables->at(_componentTypeId); }
		inline const std::vector<unsigned int>* GetChangedEntities() { return m_changedEntities; }

		inline bool EntityHasComponent(unsigned int _entityId, unsigned int _componentTypeId){ return m_entityTable->EntityHasComponent(_entityId, _componentTypeId); }
		inline unsigned int GetEntityCountLimit(){ return m_entityTable->EntityCountLimit(); }

		unsigned int GetMemoryAllocated();

	private:
		SDL_mutex* m_changedEntitiesMutex;
		SDL_mutex* m_entitiesToBeRemovedMutex;
		SDL_mutex* m_componentsToBeAddedMutex;
		SDL_mutex* m_componentsToBeRemovedMutex;
		SDL_mutex* m_componentDataToBeClearedMutex;
		unsigned int m_entityCount;
		EntityTable* m_entityTable;
		std::vector<ComponentTable*>* m_componentTables;
		std::vector<unsigned int>* m_componentTypeIds;
		std::vector<unsigned int>* m_changedEntities;
		std::vector<unsigned int>* m_entitiesToBeRemoved;
		std::map<unsigned int, std::vector<unsigned int>*>* m_componentsToBeAdded;
		std::map<unsigned int, std::vector<unsigned int>*>* m_componentsToBeRemoved;
		std::vector<EntityComponents*>* m_componentDataToBeCleared;

		void Changed(unsigned int _entityId);
		void ToBeAdded(unsigned int _entityId, unsigned int _componentTypeId);
		void ToBeRemoved(unsigned int _entityId);
		void ToBeRemoved(unsigned int _entityId, unsigned int _componentTypeId);
		void DataToBeCleared(unsigned int _entityId, unsigned int _componentTypeId);
	};
}

#endif