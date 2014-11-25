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
		EntityTable* m_entityTable;
		std::vector<DataTable*>* m_componentTables;
		std::vector<int>* m_componentTypeIds;

	public:
		explicit DataManager(unsigned int _entityCount);
		~DataManager();

		void InitializeTables();

		void AddComponentType(int _componentType);
	};
}

#endif