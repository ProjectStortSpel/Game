#include "DataManager.h"
#include "../../Managers/ComponentTypeManager.h"
#include "DataArray.h"

using namespace ECSL;

DataManager::DataManager(unsigned int _entityCount, std::vector<int>* _componentTypeIds) : m_entityCount(_entityCount), m_componentTypeIds(_componentTypeIds)
{
}

DataManager::~DataManager()
{
	for (int n = m_componentTables->size() - 1; n >= 0; --n)
		delete m_componentTables->at(n);
	delete m_componentTables;

	delete m_entityTable;
	delete m_componentTypeIds;
}

void DataManager::InitializeTables()
{
	m_componentTables = new std::vector<DataTable*>();
	m_entityTable = new EntityTable(m_entityCount, m_componentTypeIds->size());

	for (int n = 0; n < m_componentTypeIds->size(); ++n)
	{
		ComponentType* componentType = ComponentTypeManager::GetInstance().GetComponentType(m_componentTypeIds->at(n));
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