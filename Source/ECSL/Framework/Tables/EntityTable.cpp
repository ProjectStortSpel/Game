#include "EntityTable.h"

using namespace ECSL;

EntityTable::EntityTable(unsigned int _entityCount, unsigned int _componentCount)
{
	m_entityCount = _entityCount;
	m_componentCount = _componentCount;

	/* Each component state in every entity equals one bit, either it has the component (1) or it doesn't (0) */
	m_componentByteCount = BitSet::GetByteCount(_componentCount);
	m_componentIntCount = BitSet::GetIntCount(_componentCount);
	m_dataTable = new DataArray(_entityCount, 1 + m_componentIntCount * BitSet::GetIntByteSize());

	/*	TEST	*/
	BitSet::BitSetConverter& bsc = BitSet::BitSetConverter::GetInstance();

	//	Entity 0 has 1001
	std::vector<unsigned int>* entityComponents = new std::vector<unsigned int>();
	entityComponents->push_back(0);
	entityComponents->push_back(24);
	//entityComponents->push_back(45);
	entityComponents->push_back(44);
	entityComponents->push_back(64);

	BitSet::DataType* obligatoryBits = bsc.GenerateBitmask(entityComponents, _componentCount);
	BitSet::DataType test = obligatoryBits[0];
	m_dataTable->SetData(0, 1, obligatoryBits, m_componentIntCount * sizeof(BitSet::DataType));
}

EntityTable::~EntityTable()
{
	if (m_dataTable)
		delete(m_dataTable);
}

bool EntityTable::EntityHasComponent(unsigned int _entityId, std::string _componentType)
{
	BitSet::DataType* componentBitSet = (BitSet::DataType*)(m_dataTable->GetData(_entityId) + 1);
	return true;
}

bool EntityTable::EntityHasComponents(unsigned int _entityId, BitSet::DataType* _mandatoryMask, BitSet::DataType* _oneOfMask, BitSet::DataType* _exclusionMask)
{
	/* Component bit set for the entity */
	BitSet::DataType* componentBitSet = (BitSet::DataType*)(m_dataTable->GetData(_entityId) + 1);

	/* Checks every component filter (breaks if fails) */
	for (unsigned int i = 0; i < m_componentIntCount; ++i)
	{
		/* Entity doesn't have atleast one of the must-have components */
		if ( !( (_mandatoryMask[i] & componentBitSet[i]) == _mandatoryMask[i]) )
			return false;
		/* Entity has none of the atleast-one-of components */
		else if (((_oneOfMask[i] & componentBitSet[i]) == 0))
			return false;
		/* Entity has atleast one of the excluded components  */
		else if (((_exclusionMask[i] & componentBitSet[i]) != 0))
			return false;
	}
	return true;
}