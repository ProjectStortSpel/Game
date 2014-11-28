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
		EntityTable(unsigned int _entityCount, unsigned int _componentTypeCount);
		~EntityTable();

		void AddComponentTo(unsigned int _entityId, unsigned int _componentTypeId);
		bool EntityHasComponent(unsigned int _entityId, unsigned int _componentTypeId);
		bool EntityHasComponents(unsigned int _entityId, BitSet::DataType* _mandatoryMask, BitSet::DataType* _oneOfMask, BitSet::DataType* _exclusionMask);
		unsigned int GenerateNewEntityId();
		void AddOldEntityId(unsigned int _entityId);

		inline EntityState GetEntityState(unsigned int _entityId) { return (EntityState)(*(int*)(m_dataTable->GetData(_entityId))); }
		void GetEntityComponents(std::vector<unsigned int>& _out, unsigned int _entityId);
	private:
		unsigned int m_entityCount;
		unsigned int m_componentTypeCount;
		unsigned int m_componentByteCount;
		unsigned int m_componentIntCount;
		std::stack<unsigned int>* m_availableEntityIds;
		DataArray* m_dataTable;
	};
}

#endif