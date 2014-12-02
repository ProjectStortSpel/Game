#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <SDL/SDL.h>
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

		void ClearComponentData();
		void RecycleEntityIds();
		void ClearChangeLists();

		inline unsigned int GetEntityCount() { return m_entityCount; }
		inline EntityTable* GetEntityTable() { return m_entityTable; }
		inline DataTable* GetComponentTable(unsigned int _tableId) { return m_componentTables->at(_tableId); }
		inline const std::vector<unsigned int>* const GetChangedEntities() { return m_changedEntities; }

	private:
		unsigned int m_entityCount;
		EntityTable* m_entityTable;
		std::map<unsigned int, DataTable*>* m_componentTables;
		std::vector<unsigned int>* m_componentTypeIds;
		std::vector<unsigned int>* m_entitiesToBeRemoved;
		std::vector<unsigned int>* m_changedEntities;
		std::map<unsigned int, std::vector<unsigned int>>* m_componentsToBeRemoved;

		bool AddUniqueElement(unsigned int _element, std::vector<unsigned int> _list);
	};
}

#endif