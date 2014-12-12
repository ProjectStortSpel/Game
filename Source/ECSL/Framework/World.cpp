#include "World.h"
#include "../Managers/ComponentTypeManager.h"
#include "../Managers/EntityTemplateManager.h"
using namespace ECSL;

World::World(unsigned int _entityCount, std::vector<SystemWorkGroup*>* _systemWorkGroups, std::vector<unsigned int>* _componentTypeIds)
{
	m_dataManager = new DataManager(_entityCount, _componentTypeIds);
	m_systemManager = new SystemManager(m_dataManager, _systemWorkGroups);
	m_simulation = new Simulation(m_dataManager, m_systemManager);

	m_dataManager->InitializeTables();
	m_systemManager->InitializeSystems();
}

World::~World()
{
	delete(m_simulation);
	delete(m_dataManager);
	delete(m_systemManager);
}

void World::Update(float _dt)
{
	m_simulation->Update(_dt);
}

unsigned int World::CreateNewEntity()
{
	++m_activeEntities;
	return m_dataManager->CreateNewEntity();
}

void World::CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId)
{
	m_dataManager->CreateComponentAndAddTo(_componentType, _entityId);
}

void World::RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId)
{
	m_dataManager->RemoveComponentFrom(_componentType, _entityId);
}

DataLocation World::GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName)
{
	return m_dataManager->GetComponentTable(_componentType)->GetComponent(_entityId, _variableName);
}
DataLocation World::GetComponent(unsigned int _entityId, const std::string& _componentType, const int _index)
{
	return m_dataManager->GetComponentTable(_componentType)->GetComponent(_entityId, _index);
}
DataLocation World::GetComponent(unsigned int _entityId, const unsigned int _componentType, const int _index)
{
	return m_dataManager->GetComponentTable(_componentType)->GetComponent(_entityId, _index);
}

void World::SetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName, void* _data)
{
	ComponentTable* componentTable = m_dataManager->GetComponentTable(ComponentTypeManager::GetInstance().GetTableId(_componentType));
	componentTable->SetComponent(_entityId, _variableName, _data);
}

void World::KillEntity(unsigned int _entityId)
{
	--m_activeEntities;
	m_dataManager->RemoveEntity(_entityId);
}

unsigned int World::CreateNewEntity(std::string _templateName)
{
	unsigned int newId = CreateNewEntity();
	EntityTemplate* entityTemplate = EntityTemplateManager::GetInstance().GetTemplate(_templateName);
	std::map<std::string, std::vector<TemplateEntry>>* _components = entityTemplate->GetComponents();

	for (auto component : *_components)
	{
		std::string componentType = component.first;
		CreateComponentAndAddTo(componentType, newId);

		if (component.second.size() != 0)
		{
			auto componentData = component.second;
			int byteOffset = 0;
			for (int n = 0; n < componentData.size(); ++n)
			{
				if (componentData[n].GetDataType() == ComponentDataType::INT)
				{
					int* dataLoc = (int*)GetComponent(newId, componentType, byteOffset);
					dataLoc[0] = componentData[n].GetIntData();
					byteOffset += sizeof(int);
				}
				else if (componentData[n].GetDataType() == ComponentDataType::FLOAT)
				{
					float* dataLoc = (float*)GetComponent(newId, componentType, byteOffset);
					dataLoc[0] = componentData[n].GetFloatData();
					byteOffset += sizeof(float);
				}
				else if (componentData[n].GetDataType() == ComponentDataType::REFERENCE)
				{
					int* dataLoc = (int*)GetComponent(newId, componentType, byteOffset);
					dataLoc[0] = componentData[n].GetIntData();
					byteOffset += sizeof(int);
				}
				else if (componentData[n].GetDataType() == ComponentDataType::CHAR)
				{
					char* dataLoc = (char*)GetComponent(newId, componentType, byteOffset);
					std::string textData = componentData[n].GetTextData();
					for (int i = 0; i < textData.size(); ++i)
						dataLoc[i] = textData[i];
					dataLoc[textData.size()] = '\0';
					byteOffset += CHARSIZE * sizeof(char);
				}
				else if (componentData[n].GetDataType() == ComponentDataType::BOOL)
				{
					bool* dataLoc = (bool*)GetComponent(newId, componentType, byteOffset);
					dataLoc[0] = componentData[n].GetBoolData();
					byteOffset += sizeof(bool);
				}
				
			}
		}
	}

	return newId;
}

