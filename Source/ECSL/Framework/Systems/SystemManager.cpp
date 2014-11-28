#include "SystemManager.h"
#include "ECSL/Managers/ComponentTypeManager.h"
using namespace ECSL;

SystemManager::SystemManager(std::vector<SystemWorkGroup*>* _systemWorkGroups)
{
	m_systemWorkGroups = _systemWorkGroups;
}

SystemManager::~SystemManager()
{
	for (int n = m_systemWorkGroups->size() - 1; n >= 0; --n)
		delete m_systemWorkGroups->at(n);

	delete m_systemWorkGroups;
}

void SystemManager::InitializeSystems()
{
	/*	Initialize all system groups	*/
	for (unsigned int groupId = 0; groupId < m_systemWorkGroups->size(); ++groupId)
	{
		std::vector<System*>* systems = m_systemWorkGroups->at(groupId)->GetSystems();

		/*	Go through all systems in the group and initialize them	*/
		for (unsigned int systemId = 0; systemId < systems->size(); ++systemId)
		{
			systems->at(systemId)->Initialize();

			//GenerateSystemBitmask(systems->at(systemId), ComponentFilter::Mandatory);
			//GenerateSystemBitmask(systems->at(systemId), ComponentFilter::RequiresOneOf);
			//GenerateSystemBitmask(systems->at(systemId), ComponentFilter::Excluded);
		}
	}


}

void SystemManager::GenerateSystemBitmask(System* _system, ComponentFilter _type)
{
	SystemBitmask* tSystemBitmask = _system->GetSystemBitmask(_type);

	std::vector<std::string>* componentTypes = tSystemBitmask->GetComponentTypes();
	std::vector<unsigned int> componentIds = std::vector<unsigned int>();
	ComponentTypeManager componentTypeManager = ComponentTypeManager::GetInstance();
	for (unsigned int i = 0; i < componentTypes->size(); ++i)
	{
		unsigned int componentTypeId = componentTypeManager.GetTableId(componentTypes->at(i));
		componentIds.push_back(componentTypeId);
	}
	unsigned int componentTypeCount = componentTypeManager.GetComponentTypeCount();
	BitSet::DataType* convertedBitmask = 0;
	BitSet::BitSetConverter::GetInstance().ArrayToBitSet(convertedBitmask, componentIds, componentTypeCount);
	tSystemBitmask->SetBitmask(convertedBitmask);
}