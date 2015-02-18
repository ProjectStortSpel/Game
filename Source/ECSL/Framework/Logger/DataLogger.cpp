#include "DataLogger.h"

#include <sstream>
#include "Logger/Managers/Logger.h"

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

bool DataLogger::LogWorldDataAssert(LogType _whatToLog)
{
	LogWorldData(_whatToLog);
	return false;
}

void DataLogger::LogWorldData(LogType _whatToLog)
{
	std::string message = NewLine;
	switch (_whatToLog)
	{
	case LogType::EVERYTHING:
		CreateSystemEntitiesMessage(message);
		message += NewLine;
		CreateComponentTablesMessage(message);
		message += NewLine;
		CreateEntityComponentsMessage(message);
		break;
	case LogType::SYSTEM_ENTITIES:
		CreateSystemEntitiesMessage(message);
		break;
	case LogType::COMPONENT_TABLES:
		CreateComponentTablesMessage(message);
		break;
	case ENTITY_COMPONENTS:
		CreateEntityComponentsMessage(message);
		break;
	}
	std::stringstream group;
	group << clock();
	Logger::GetInstance().Log(Logger::GetInstance().AddGroup(group.str(), false), LogSeverity::Info, message);
}

void DataLogger::CreateSystemEntitiesMessage(std::string& _out)
{
	int groupIndex = -1;
	std::stringstream message;
	message << "Systems: " << NewLine << NewLine;
	for (const auto systemGroup : *m_systemWorkGroups)
	{
		message << "Group: " << ++groupIndex << NewLine;
		for (const auto system : *systemGroup->GetSystems())
		{
			message << "(Id: " << system->GetId() << ") " << system->GetSystemName() << ": ";
			for (const auto entityId : *system->GetEntities())
			{
					message << entityId << " ";
			}
			message << NewLine;
		}
		message << NewLine << NewLine;
	}
	_out += message.str();
}

void DataLogger::CreateComponentTablesMessage(std::string& _out)
{
	std::stringstream message;
	message << "Components: " << NewLine << NewLine;
	ComponentTypeManager* componentTypeManager = &ComponentTypeManager::GetInstance();
	for (unsigned int i = 0; i < m_componentTables->size(); ++i)
	{
		ComponentTable* componentTable = (*m_componentTables)[i];
		
		if (componentTable)
		{
			message << "(Id: " << i << ") " << componentTypeManager->GetComponentType(i)->GetName() << ": ";
			for (unsigned int entityId = 0; entityId < m_entityCount; ++entityId)
			{
				std::vector<unsigned int> components;
				if (m_entityTable->HasComponent(entityId, i))
					message << entityId << " ";
			}
			message << NewLine;
		}
	}
	_out += message.str();
}

void DataLogger::CreateEntityComponentsMessage(std::string& _out)
{
	std::stringstream message;
	message << "Entities: " << NewLine;
	ComponentTypeManager* componentTypeManager = &ComponentTypeManager::GetInstance();
	for (unsigned int entityId = 0; entityId < m_entityCount; ++entityId)
	{
		std::vector<unsigned int> components;
		m_entityTable->GetEntityComponents(components, entityId);
		if (components.size() > 0)
		{
			message << "(Id: " << entityId << ") ";
			for (unsigned int componentId = 0; componentId < components.size(); ++componentId)
			{
				if (componentId != components.size() - 1)
					message << componentTypeManager->GetComponentType(components[componentId])->GetName() << ", ";
				else
					message << componentTypeManager->GetComponentType(components[componentId])->GetName();
			}
			message << NewLine;
		}
	}
	_out += message.str();
}