#ifndef ENTITYTABLE_H
#define ENTITYTABLE_H

#include <SDL/SDL.h>
#include <SDL/SDL_mutex.h>
#include <vector>
#include <string>

#include "DataArray.h"
#include "ECSL/Framework/Common/BitSet.h"

namespace ECSL
{
	enum EntityState
	{
		Dead = 0,
		Alive = 1
	};

	class DECLSPEC EntityTable
	{
	public:
		EntityTable(unsigned int _entityCount, unsigned int _componentTypeCount);
		~EntityTable();

		void AddComponentTo(unsigned int _entityId, unsigned int _componentTypeId);
		void AddComponentsTo(unsigned int _entityId, const std::vector<unsigned int>& _componentTypeIds);
		void RemoveComponentFrom(unsigned int _entityId, unsigned int _componentTypeId);
		void RemoveComponentsFrom(unsigned int _entityId, const std::vector<unsigned int>& _componentTypeIds);
		bool HasComponent(unsigned int _entityId, unsigned int _componentTypeId);
		unsigned int GenerateNewEntityId();
		void AddOldEntityId(unsigned int _entityId);
		void ClearEntityData(unsigned int _entityId);

		inline const EntityState GetEntityState(unsigned int _entityId);
		inline const BitSet::DataType* GetEntityComponents(unsigned int _entityId);
		void GetEntityComponents(std::vector<unsigned int>& _out, unsigned int _entityId);

	private:
		unsigned int m_entityCount;
		unsigned int m_componentTypeCount;
		unsigned int m_componentByteCount;
		unsigned int m_componentIntCount;
		std::vector<unsigned int>* m_availableEntityIds;
		SDL_mutex* m_availableEntityMutex;
		DataArray* m_dataTable;
	};
}

#endif