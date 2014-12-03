#include "System.h"

#include <assert.h>
#include <map>
#include <vector>

using namespace ECSL;

System::System() : m_systemType(SystemType::Both)
{
	
}

System::~System()
{
	if (m_entities)
		delete(m_entities);
}

void System::InitializeEntityList()
{
	m_entities = BitSet::GenerateBitSet(m_dataManager->GetEntityCount());
}

void System::AddEntity(unsigned int _entityId)
{
	unsigned int bitIndex = ECSL::BitSet::GetBitIndex(_entityId);
	unsigned int bitSetIndex = ECSL::BitSet::GetBitSetIndex(_entityId);
	m_entities[bitSetIndex] |= ((ECSL::BitSet::DataType)1) << bitIndex;
}

void System::RemoveEntity(unsigned int _entityId)
{
	unsigned int bitIndex = ECSL::BitSet::GetBitIndex(_entityId);
	unsigned int bitSetIndex = ECSL::BitSet::GetBitSetIndex(_entityId);
	m_entities[bitSetIndex] &= ~(((ECSL::BitSet::DataType)1) << bitIndex);
}

bool System::HasEntity(unsigned int _entityId)
{
	unsigned int bitIndex = ECSL::BitSet::GetBitIndex(_entityId);
	unsigned int bitSetIndex = ECSL::BitSet::GetBitSetIndex(_entityId);
	if (m_entities[bitSetIndex] & ((ECSL::BitSet::DataType)1) << bitIndex)
		return true;
	return false;
}

void System::AddComponentTypeToFilter(const std::string& _componentType, FilterType _filterType)
{
	switch (_filterType)
	{
	case FilterType::Mandatory:
		m_mandatoryComponentTypes.Add(_componentType);
		break;
	case FilterType::RequiresOneOf:
		m_requiresOneOfComponentTypes.Add(_componentType);
		break;
	case FilterType::Excluded:
		m_excludedComponentTypes.Add(_componentType);
		break;
	default:
		printf("Unknown ComponentFilter passed into AddComponentToFilter [System.cpp]\n");
		break;
	}
}