#include "EntityTable.h"
#include "ECSL/Managers/ComponentTypeManager.h"
#include <assert.h>

using namespace ECSL;

EntityTable::EntityTable(unsigned int _entityCount, unsigned int _componentTypeCount) : m_entityCount(_entityCount), m_componentTypeCount(_componentTypeCount)
{
	m_availableEntityMutex = SDL_CreateMutex();

	/* Each component state in every entity equals one bit, either it has the component (1) or it doesn't (0) */
	m_componentByteCount = BitSet::GetByteCount(_componentTypeCount);
	m_componentIntCount = BitSet::GetDataTypeCount(_componentTypeCount);
	m_dataTable = new DataArray(_entityCount, 1 + m_componentIntCount * BitSet::GetDataTypeByteSize());

	/* All entity id slots are available from the beginning */
	m_availableEntityIds = new std::vector<unsigned int>();
	for (int i = _entityCount - 1; i >= 0; --i)
		m_availableEntityIds->push_back(i);
}

EntityTable::~EntityTable()
{
	delete(m_dataTable);
	delete(m_availableEntityIds);
	SDL_DestroyMutex(m_availableEntityMutex);
}

void EntityTable::AddComponentTo(unsigned int _entityId, unsigned int _componentTypeId)
{
	BitSet::DataType* entityComponents = (BitSet::DataType*)(m_dataTable->GetData(_entityId, 1));
	unsigned int bitSetIndex = BitSet::GetBitSetIndex(_componentTypeId);
	unsigned int bitIndex = BitSet::GetBitIndex(_componentTypeId);

	/* The entity is dead */
	assert(!(*(unsigned char*)m_dataTable->GetData(_entityId) == 0));

	/* The component is already added to the entity */
	assert(!(entityComponents[bitSetIndex] & ((BitSet::DataType)1 << bitIndex)));

	/* Add component to entity */
	entityComponents[bitSetIndex] |= (BitSet::DataType)1 << bitIndex;
}

void EntityTable::AddComponentsTo(unsigned int _entityId, const std::vector<unsigned int>& _componentTypeIds)
{
	/* The entity is dead */
	assert(!(*(unsigned char*)m_dataTable->GetData(_entityId) == 0));

	BitSet::DataType* entityComponents = (BitSet::DataType*)(m_dataTable->GetData(_entityId, 1));
	/* Adding components to entity */
	for (unsigned int componentTypeId : _componentTypeIds)
	{
		unsigned int bitSetIndex = BitSet::GetBitSetIndex(componentTypeId);
		unsigned int bitIndex = BitSet::GetBitIndex(componentTypeId);
		
		/* The component is already added to the entity */
		assert(!(entityComponents[bitSetIndex] & ((BitSet::DataType)1 << bitIndex)));

		/* Add component to entity */
		entityComponents[bitSetIndex] |= (BitSet::DataType)1 << bitIndex;
	}
}

void EntityTable::RemoveComponentFrom(unsigned int _entityId, unsigned int _componentTypeId)
{
	BitSet::DataType* entityComponents = (BitSet::DataType*)(m_dataTable->GetData(_entityId, 1));
	unsigned int bitSetIndex = BitSet::GetBitSetIndex(_componentTypeId);
	unsigned int bitIndex = BitSet::GetBitIndex(_componentTypeId);

	/* The entity is dead */
	assert(!(*(unsigned char*)m_dataTable->GetData(_entityId) == 0));

	/* The entity doesn't have that component */
	assert(entityComponents[bitSetIndex] & ((BitSet::DataType)1 << bitIndex));

	/* Remove component from entity */
	entityComponents[bitSetIndex] &= ~((BitSet::DataType)1 << bitIndex);
}

void EntityTable::RemoveComponentsFrom(unsigned int _entityId, const std::vector<unsigned int>& _componentTypeIds)
{
	/* The entity is dead */
	assert(!(*(unsigned char*)m_dataTable->GetData(_entityId) == 0));

	BitSet::DataType* entityComponents = (BitSet::DataType*)(m_dataTable->GetData(_entityId, 1));
	/* Removing components from entity */
	for (unsigned int componentTypeId : _componentTypeIds)
	{
		unsigned int bitSetIndex = BitSet::GetBitSetIndex(componentTypeId);
		unsigned int bitIndex = BitSet::GetBitIndex(componentTypeId);

		/* The entity doesn't have that component */
		assert(entityComponents[bitSetIndex] & ((BitSet::DataType)1 << bitIndex));

		/* Remove component from entity */
		entityComponents[bitSetIndex] &= ~((BitSet::DataType)1 << bitIndex);
	}
}

bool EntityTable::HasComponent(unsigned int _entityId, unsigned int _componentTypeId)
{
	BitSet::DataType* componentBitSet = (BitSet::DataType*)(m_dataTable->GetData(_entityId, 1));
	unsigned int bitSetIndex = BitSet::GetBitSetIndex(_componentTypeId);
	unsigned int bitIndex = BitSet::GetBitIndex(_componentTypeId);
	return ((componentBitSet[bitSetIndex]) & ((BitSet::DataType)1 << (bitIndex))) != 0;
}

unsigned int EntityTable::GenerateNewEntityId()
{
	/* Max entity count reached */
	assert(m_availableEntityIds->size() != 0);

	SDL_LockMutex(m_availableEntityMutex);
	/* Fetch entity id from the queue */
	unsigned int id = m_availableEntityIds->back();
	m_availableEntityIds->pop_back();
	SDL_UnlockMutex(m_availableEntityMutex);

	unsigned char alive = ((unsigned char)EntityState::Alive);
	/* Change the entity state to alive */
	m_dataTable->SetData(id, &alive, 1);

	return id;
}

void EntityTable::AddOldEntityId(unsigned int _entityId)
{
	/* Entity id is bigger than the entity count */
	assert(_entityId < m_entityCount);

	m_availableEntityIds->push_back(_entityId);
}

void EntityTable::ClearEntityData(unsigned int _entityId)
{
	m_dataTable->ClearRow(_entityId);
}

void EntityTable::GetEntityComponents(std::vector<unsigned int>& _out, unsigned int _entityId)
{
	BitSet::DataType* entityComponents = (BitSet::DataType*)(m_dataTable->GetData(_entityId, 1));
	const unsigned int bitCount = BitSet::GetDataTypeByteSize() * 8;
	for (unsigned int bitSetIndex = 0; bitSetIndex < m_componentIntCount; ++bitSetIndex)
	{
		if (entityComponents[bitSetIndex] == 0)
			continue;
		for (unsigned int bitIndex = 0; bitIndex < bitCount; ++bitIndex)
		{
			if (entityComponents[bitSetIndex] & ((BitSet::DataType)1 << (bitIndex)))
				_out.push_back(bitSetIndex * bitCount + bitIndex);
		}
	}
}