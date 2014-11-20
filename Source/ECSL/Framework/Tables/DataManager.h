#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <SDL/SDL.h>
#include "DataTable.h"
#include "EntityTable.h"

#include <vector>

namespace ECSL
{
	class DECLSPEC DataManager
	{
	private:
		unsigned int m_entityCount;
		unsigned int m_componentCount;
		EntityTable* m_entityTable;
		std::vector<DataTable>* m_componentTables;

	public:
		explicit DataManager(unsigned int _entityCount);
		~DataManager();

		void InitializeTables();

		template<typename ComponentType>
		void AddComponentType();
		void AddComponentType(std::string _componentType);
	};

	template<typename ComponentType>
	void DataManager::AddComponentType()
	{
		++m_componentCount;
	}
}

#endif