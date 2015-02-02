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
		enum ComponentChange
		{
			TO_BE_ADDED,
			TO_BE_REMOVED
		};

		struct EntityChange
		{
			bool Dead;
			std::map<unsigned int, ComponentChange> ComponentChanges;

			EntityChange() : Dead(false) { }
		};

		DataManager(unsigned int _entityCount, std::vector<unsigned int>* _componentTypeIds);
		~DataManager();

		void InitializeTables();

		unsigned int CreateNewEntity();
		void RemoveEntity(unsigned int _entityId);
		void CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId);
		void CreateComponentAndAddTo(unsigned int _componentTypeId, unsigned int _entityId);
		void RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId);
		void RemoveComponentFrom(unsigned int _componentTypeId, unsigned int _entityId);

		/* Update all component changes to entities in the entity table */
		void UpdateEntityTable(const RuntimeInfo& _runtime);

		/* Recycle the ids of dead entities */
		void RecycleEntityIds(const RuntimeInfo& _runtime);

		/* Copy entity change lists */
		void CopyCurrentLists(const RuntimeInfo& _runtime);

		/* Clear the copied lists */
		void ClearCopiedLists(const RuntimeInfo& _runtime);

		/* Clear the data from removed components from entities */
		void DeleteComponentData(const RuntimeInfo& _runtime);

		inline unsigned int GetEntityCount() { return m_entityCount; }
		inline unsigned int GetComponentTypeCount() { return m_componentTypeCount; }
		inline EntityTable* GetEntityTable() { return m_entityTable; }
		inline ComponentTable* GetComponentTable(const std::string& _componentType) { return m_componentTables->at(ComponentTypeManager::GetInstance().GetTableId(_componentType)); }
		inline ComponentTable* GetComponentTable(unsigned int _componentTypeId) { return m_componentTables->at(_componentTypeId); }
		inline const std::map<unsigned int, EntityChange*>* GetEntityChanges() { return m_entityChangesCopy; }

		inline bool HasComponent(unsigned int _entityId, unsigned int _componentTypeId){ return m_entityTable->HasComponent(_entityId, _componentTypeId); }

		unsigned int GetMemoryAllocated();

	private:
		SDL_mutex* m_entityChangesMutex;
		unsigned int m_entityCount;
		unsigned int m_componentTypeCount;
		EntityTable* m_entityTable;
		std::vector<ComponentTable*>* m_componentTables;
		std::vector<unsigned int>* m_componentTypeIds;
		std::map<unsigned int, EntityChange*>* m_entityChanges;
		std::map<unsigned int, EntityChange*>* m_entityChangesCopy;

		void ToBeAdded(unsigned int _entityId, unsigned int _componentTypeId);
		void ToBeRemoved(unsigned int _entityId);
		void ToBeRemoved(unsigned int _entityId, unsigned int _componentTypeId);
	};
}

#endif