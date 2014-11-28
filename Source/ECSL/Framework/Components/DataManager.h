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
		explicit DataManager(unsigned int _entityCount, std::vector<unsigned int>* _componentTypeIds);
		~DataManager();

		void InitializeTables();

		int CreateNewEntity();
		void CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId);
		void KillEntity(unsigned int _entityId);

	private:
		unsigned int m_entityCount;
		EntityTable* m_entityTable;
		std::map<unsigned int, DataTable*>* m_componentTables;
		std::vector<unsigned int>* m_componentTypeIds;
	};
}

#endif