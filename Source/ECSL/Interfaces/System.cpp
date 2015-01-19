#include "System.h"

#include "ECSL/Framework/Common/ContainerHelper.h"
#include <assert.h>
#include <map>
#include <vector>

using namespace ECSL;

System::System()
:	m_systemName(new std::string()),
	m_subscriptions(new std::vector<Subscription*>()),
	m_messages(new std::vector<Message*>())
{
	m_messagesMutex = SDL_CreateMutex();
}

System::~System()
{
	if (m_systemName)
		delete(m_systemName);
	if (m_entitiesBitSet)
		delete(m_entitiesBitSet);
	if (m_entities)
		delete(m_entities);
	if (m_subscriptions)
	{
		for (auto subscription : *m_subscriptions)
			delete(subscription);
		delete(m_subscriptions);
	}
	if (m_messages)
	{
		for (unsigned int i = 0; i < m_messages->size(); ++i)
			delete(m_messages->at(i));
		delete(m_messages);
	}
	SDL_DestroyMutex(m_messagesMutex);
}

void System::InitializeEntityList()
{
	m_entities = new std::vector<unsigned int>();
	m_entitiesBitSet = BitSet::GenerateBitSet(m_dataManager->GetEntityCount());
}

void System::AddEntityToSystem(unsigned int _entityId)
{
	unsigned int bitIndex = ECSL::BitSet::GetBitIndex(_entityId);
	unsigned int bitSetIndex = ECSL::BitSet::GetBitSetIndex(_entityId);
	m_entitiesBitSet[bitSetIndex] |= ((ECSL::BitSet::DataType)1) << bitIndex;
	m_entities->push_back(_entityId);
}

void System::RemoveEntityFromSystem(unsigned int _entityId)
{
	unsigned int bitIndex = ECSL::BitSet::GetBitIndex(_entityId);
	unsigned int bitSetIndex = ECSL::BitSet::GetBitSetIndex(_entityId);
	m_entitiesBitSet[bitSetIndex] &= ~(((ECSL::BitSet::DataType)1) << bitIndex);
	ContainerHelper::RemoveUniqueElement<unsigned int>(_entityId, *m_entities);
}

bool System::HasEntity(unsigned int _entityId)
{
	unsigned int bitIndex = ECSL::BitSet::GetBitIndex(_entityId);
	unsigned int bitSetIndex = ECSL::BitSet::GetBitSetIndex(_entityId);
	if (m_entitiesBitSet[bitSetIndex] & ((ECSL::BitSet::DataType)1) << bitIndex)
		return true;
	return false;
}

DataLocation System::GetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName)
{
	return m_dataManager->GetComponentTable(_componentType)->GetComponent(_entityId, _variableName);
}

DataLocation System::GetComponent(unsigned int _entityId, const std::string& _componentType, unsigned int _index)
{
	return m_dataManager->GetComponentTable(_componentType)->GetComponent(_entityId, _index);
}

DataLocation System::GetComponent(unsigned int _entityId, unsigned int _componentTypeId, unsigned int _index)
{
	return m_dataManager->GetComponentTable(_componentTypeId)->GetComponent(_entityId, _index);
}

void System::SetComponent(unsigned int _entityId, const std::string& _componentType, const std::string& _variableName, void* _data)
{
	m_dataManager->GetComponentTable(_componentType)->SetComponent(_entityId, _variableName, _data);
}

void System::SetComponent(unsigned int _entityId, unsigned int _componentTypeId, unsigned int _index, void* _data, unsigned int _byteSize)
{
	m_dataManager->GetComponentTable(_componentTypeId)->SetComponent(_entityId, _index, _data, _byteSize);
}

void System::CreateComponentAndAddTo(const std::string& _componentType, unsigned int _entityId)
{
	m_dataManager->CreateComponentAndAddTo(_componentType, _entityId);
}

void System::CreateComponentAndAddTo(unsigned int _componentTypeId, unsigned int _entityId)
{
	m_dataManager->CreateComponentAndAddTo(_componentTypeId, _entityId);
}

void System::RemoveComponentFrom(const std::string& _componentType, unsigned int _entityId)
{
	m_dataManager->RemoveComponentFrom(_componentType, _entityId);
}

void System::RemoveComponentFrom(unsigned int _componentTypeId, unsigned int _entityId)
{
	m_dataManager->RemoveComponentFrom(_componentTypeId, _entityId);
}

ComponentTable* System::GetComponentTable(const std::string& _componentType)
{
	return m_dataManager->GetComponentTable(_componentType);
}

ComponentTable* System::GetComponentTable(unsigned int _componentTypeId)
{
	return m_dataManager->GetComponentTable(_componentTypeId);
}

unsigned int System::GetComponentVariableIndex(const std::string& _componentType, const std::string& _variableName)
{
	return ComponentTypeManager::GetInstance().GetComponentType(ComponentTypeManager::GetInstance().GetTableId(_componentType))->GetVariables()->find(_variableName)->second.GetOffset();
}

unsigned int System::CreateNewEntity()
{
	return m_dataManager->CreateNewEntity();
}

void System::KillEntity(unsigned int _entityId)
{
	m_dataManager->RemoveEntity(_entityId);
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

void System::SubscribeTo(const std::string& _systemName, unsigned int _messageType)
{
	m_subscriptions->push_back(new Subscription(*m_systemName, _systemName, _messageType));
}

void System::SendMessage(Message* _message)
{
	SDL_LockMutex(m_messagesMutex);
	m_messages->push_back(_message);
	SDL_UnlockMutex(m_messagesMutex);
}