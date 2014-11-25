#include "ComponentManager.h"
using namespace ECSL;


ComponentTypeManager& ComponentTypeManager::GetInstance()
{
	static ComponentTypeManager* instance = new ComponentTypeManager();
	return *instance;
}
ComponentTypeManager::ComponentTypeManager()
	: m_loadedComponentTypes(new std::map<int, ComponentTypeInterface*>())
{
}

ComponentTypeInterface* ComponentTypeManager::GetComponentType(int _componentTypeId)
{
	//	Check if the given Component Type Id is loaded,
	//	if it is return the correct object, otherwise
	//	return null pointer
	auto it = m_loadedComponentTypes->find(_componentTypeId);
	if (it != m_loadedComponentTypes->end())
		return it->second;
	return 0;
}