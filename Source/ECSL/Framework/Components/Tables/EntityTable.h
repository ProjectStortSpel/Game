#ifndef ENTITYTABLE_H
#define ENTITYTABLE_H

#include <SDL/SDL.h>
#include <stack>
#include <string>

#include "DataArray.h"
#include "../../Common/BitSet.h"

namespace ECSL
{
	enum EntityState
	{
		Dead,
		Alive
	};

	class DECLSPEC EntityTable
	{
	public:
		EntityTable(unsigned int _entityCount, unsigned int _componentCount);
		~EntityTable();

		inline EntityState GetEntityState(unsigned int _entityId) { return (EntityState)(*(int*)(m_dataTable->GetData(_entityId))); }
		inline bool EntityHasComponent(unsigned int _entityId, std::string _componentType);
		inline bool EntityHasComponents(unsigned int _entityId, BitSet::DataType* _mandatoryMask, BitSet::DataType* _oneOfMask, BitSet::DataType* _exclusionMask);
		inline unsigned int GenerateNewEntityId();
		inline void AddOldEntityId(unsigned int _id);

	private:
		DataArray* m_dataTable;
		unsigned int m_entityCount;
		unsigned int m_componentCount;
		unsigned int m_componentByteCount;
		unsigned int m_componentIntCount;
		std::stack<unsigned int>* m_availableEntityIds;
	};
}

#endif