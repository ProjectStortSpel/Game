#include "ComponentTable.h"

using namespace ECSL;

ComponentTable::ComponentTable(DataTable* _dataTable, unsigned int _componentTypeId)
:	m_dataTable(_dataTable), m_componentTypeId(_componentTypeId)
{
	m_componentType = ComponentTypeManager::GetInstance().GetComponentType(_componentTypeId);
}

ComponentTable::~ComponentTable()
{
	delete(m_dataTable);
}

DataLocation ComponentTable::GetComponent(unsigned int _entityId, const std::string& _variableName)
{
	ComponentVariable componentVariable = ComponentTypeManager::GetInstance().GetComponentType(m_componentTypeId)->GetVariables()->find(_variableName)->second;
	return GetComponent(_entityId, componentVariable.GetOffset());
}

DataLocation ComponentTable::GetComponent(unsigned int _entityId, unsigned int _index)
{
	return m_dataTable->GetData(_entityId, _index);
}

void ComponentTable::GetComponents(std::vector<DataLocation>& _out, const std::vector<unsigned int>* _entityIds, const std::string& _variableName)
{
	unsigned int index = ComponentTypeManager::GetInstance().GetComponentType(m_componentTypeId)->GetVariables()->find(_variableName)->second.GetOffset();
	_out.resize(_entityIds->size());
	unsigned int entityIndex = 0;
	for (const auto entityId : *_entityIds)
	{
		_out[index] = m_dataTable->GetData(entityId, index);
		++entityIndex;
	}
}

void ComponentTable::GetComponents(std::vector<DataLocation>& _out, const std::vector<unsigned int>* _entityIds, unsigned int _index)
{
	_out.resize(_entityIds->size());
	unsigned int index = 0;
	for (const auto entityId : *_entityIds)
	{
		_out[index] = m_dataTable->GetData(entityId, _index);
		++index;
	}
}

void ComponentTable::GetComponents(std::vector<DataLocation>& _out, const unsigned int* entities, unsigned int entityCount, const std::string& _variableName)
{
	unsigned int index = ComponentTypeManager::GetInstance().GetComponentType(m_componentTypeId)->GetVariables()->find(_variableName)->second.GetOffset();
	_out.resize(entityCount);
	for (unsigned int i = 0; i < entityCount; ++i)
	{
		_out[i] = m_dataTable->GetData(entities[i], index);
	}
}

void ComponentTable::GetComponents(std::vector<DataLocation>& _out, const unsigned int* entities, unsigned int entityCount, unsigned int _index)
{
	_out.resize(entityCount);
	for (unsigned int i = 0; i < entityCount; ++i)
	{
		_out[i] = m_dataTable->GetData(entities[i], _index);
	}
}

void ComponentTable::SetComponent(unsigned int _entityId, const std::string& _variableName, void* _data)
{
	ComponentVariable componentVariable = m_componentType->GetVariables()->find(_variableName)->second;
	SetComponent(_entityId, componentVariable.GetOffset(), _data, componentVariable.GetByteSize());
}

void ComponentTable::SetComponent(unsigned int _entityId, unsigned int _index, void* _data, unsigned int _byteSize)
{
	m_dataTable->SetData(_entityId, _index, _data, _byteSize);
}

void ComponentTable::ClearComponent(unsigned int _entityId)
{
	m_dataTable->ClearRow(_entityId);
}

unsigned int ComponentTable::GetMemoryAllocated()
{
	return m_dataTable->GetMemoryAllocated();
}