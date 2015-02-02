#include "DataLogger.h"

#include <sstream>

using namespace ECSL;

DataLogger::DataLogger()
: m_componentTables(0), m_systemWorkGroups(0)
{

}

DataLogger& DataLogger::GetInstance()
{
	static DataLogger* instance = new DataLogger();
	return *instance;
}

DataLogger::~DataLogger()
{

}

void DataLogger::SetCurrentWorld(unsigned int _entityCount, EntityTable* _entityTable, const std::vector<ComponentTable*>* _componentTables, const std::vector<SystemWorkGroup*>* _systemWorkGroups)
{
	m_entityCount = _entityCount;
	m_entityTable = _entityTable;
	m_componentTables = _componentTables;
	m_systemWorkGroups = _systemWorkGroups;
}

void DataLogger::WriteToLog(LogType _whatToLog)
{
	std::string message = "";
	switch (_whatToLog)
	{
	case LogType::EVERYTHING:
		CreateSystemEntitiesMessage(message);
		CreateComponentTablesMessage(message);
		CreateEntityComponents(message);
		break;
	}
}

void DataLogger::CreateSystemEntitiesMessage(std::string& _out)
{
	int groupIndex = -1;
	std::stringstream message;
	for (const auto systemGroup : *m_systemWorkGroups)
	{
		message << "Group: " << ++groupIndex << NewLine;
		for (const auto system : *systemGroup->GetSystems())
		{
			unsigned int entityIndex = 0;
			message << system->GetSystemName() << NewLine;
			for (const auto entityId : *system->GetEntities())
			{
				/* Sometimes, add a new row */
				if (entityIndex % 30 == 0)
					message << NewLine << entityId;
				else
					message << entityId << " ";
				++entityIndex;
			}
			message << NewLine << NewLine;
		}
		message << NewLine << NewLine << NewLine;
	}
	_out += message.str();
}

void DataLogger::CreateComponentTablesMessage(std::string& _out)
{
	std::stringstream message;
	ComponentTypeManager* componentTypeManager = &ComponentTypeManager::GetInstance();
	for (unsigned int i = 0; i < m_componentTables->size(); ++i)
	{
		ComponentTable* componentTable = (*m_componentTables)[i];
		
		if (componentTable)
		{
			message << "(Id: " << i << ") " << componentTypeManager->GetComponentType(i)->GetName() << NewLine;
			for (unsigned int entityId = 0; entityId < m_entityCount; ++entityId)
			{
				std::vector<unsigned int> components;
				m_entityTable->GetEntityComponents(components, entityId);
				for (auto componentTypeId : components)
				{
					if (entityId % 30 == 0)
						message << NewLine << componentTypeId;
					else
						message << componentTypeId << " ";
				}
			}
			message << NewLine << NewLine;
		}
	}
	_out += message.str();
}

void DataLogger::CreateEntityComponents(std::string& _out)
{
	std::stringstream message;
	for (unsigned int entityId = 0; entityId < m_entityCount; ++entityId)
	{
		std::vector<unsigned int> components;
		m_entityTable->GetEntityComponents(components, entityId);
		if (components.size() > 0)
		{
			message << "Entity: " << entityId << NewLine;
			for (auto component : components)
			{
				message << component << "";
			}
			message << NewLine << NewLine;
		}
	}
	_out += message.str();
}