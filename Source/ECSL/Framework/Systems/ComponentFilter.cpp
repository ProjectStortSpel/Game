#include "ComponentFilter.h"

using namespace ECSL;

ComponentFilter::ComponentFilter()
{
	m_addedComponentTypes = new std::vector<std::string>();
	m_bitSet = 0;
}
ComponentFilter::~ComponentFilter()
{
	if (m_addedComponentTypes)
		delete(m_addedComponentTypes);

	if (m_bitSet)
		free(m_bitSet);
}

void ComponentFilter::Add(const std::string& _componentType)
{
	/*	Check if the component type is already added	*/
	for (unsigned int n = 0; n < m_addedComponentTypes->size(); ++n)
	if (_componentType == m_addedComponentTypes->at(n))
		return;

	/*	Add the component type	*/
	m_addedComponentTypes->push_back(_componentType);
}

void ComponentFilter::SetBitSet(BitSet::DataType* _bitSet)
{
	m_bitSet = _bitSet; 
	if (m_addedComponentTypes)
	{
		delete(m_addedComponentTypes);
		m_addedComponentTypes = 0;
	}
}