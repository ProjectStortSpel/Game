#include "DataManager.h"

#include "DataArray.h"

using namespace ECSL;

DataManager::DataManager(unsigned int _entityCount) : m_entityCount(_entityCount), m_componentCount(0)
{
}

DataManager::~DataManager()
{
}

void DataManager::InitializeTables()
{
	m_componentTables = new std::vector<DataTable>();
	m_entityTable = new EntityTable(m_entityCount, m_componentCount);

}

void DataManager::AddComponentType(std::string _componentType)
{
	++m_componentCount;
}