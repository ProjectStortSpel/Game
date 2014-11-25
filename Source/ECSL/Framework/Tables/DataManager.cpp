#include "DataManager.h"
#include "ECSL/Managers/ComponentManager.h"
#include "DataArray.h"

using namespace ECSL;

DataManager::DataManager(unsigned int _entityCount) : m_entityCount(_entityCount), m_componentTypeIds(new std::vector<int>())
{
}

DataManager::~DataManager()
{
}

void DataManager::InitializeTables()
{
	m_componentTables = new std::vector<DataTable*>();
	m_entityTable = new EntityTable(m_entityCount, m_componentTypeIds->size());

	for (int n = 0; n < m_componentTypeIds->size(); ++n)
	{
		ComponentTypeInterface* componentType = ComponentTypeManager::GetInstance().GetComponentType(m_componentTypeIds->at(n));
		if (!componentType)
		{
			printf("ERROR: Invalid ComponentType! (ID %d)\n", m_componentTypeIds->at(n));
			continue;
		}


		switch (componentType->GetTableType())
		{
		case TableType::Array:
			m_componentTables->push_back(new DataArray(m_entityCount, componentType->GetByteSize()));
			break;

		case TableType::Map:
			printf("WARNING: Map not implemented yet! (ID %d)\n", m_componentTypeIds->at(n));
			break;

		default:
			printf("ERROR: Invalid Table Type! (ID %d)\n", m_componentTypeIds->at(n));
			break;
		}
	}

}

void DataManager::AddComponentType(int _componentType)
{
	for (int n = 0; n < m_componentTypeIds->size(); ++n)
		if (m_componentTypeIds->at(n) == _componentType)
			return;


	m_componentTypeIds->push_back(_componentType);
}