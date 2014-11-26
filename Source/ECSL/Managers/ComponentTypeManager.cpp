#include "ComponentTypeManager.h"

#include "../Framework/Common/FileHelper.h"

using namespace ECSL;

ComponentTypeManager& ComponentTypeManager::GetInstance()
{
	static ComponentTypeManager* instance = new ComponentTypeManager();
	return *instance;
}
ComponentTypeManager::ComponentTypeManager()
	: m_loadedComponentTypes(new std::map<int, ComponentType*>())
{
}

ComponentType* ComponentTypeManager::GetComponentType(int _componentTypeId)
{
	//	Check if the given Component Type Id is loaded,
	//	if it is return the correct object, otherwise
	//	return null pointer
	auto it = m_loadedComponentTypes->find(_componentTypeId);
	if (it != m_loadedComponentTypes->end())
		return it->second;
	return 0;
}

void ComponentTypeManager::AddComponentTypesFromDirectory(const std::string& _directoryPath)
{
	std::vector<std::string> filePaths;
	FileHelper::GetFilesInDirectory(filePaths, _directoryPath, "cmp");
}

void ComponentTypeManager::AddComponentTypesFromFile(const std::string& _filePath)
{

}

TypeId ComponentTypeManager::GetTableId(const std::string& _componentType)
{
	static std::unordered_map<std::string, TypeId> m_stringTableId = std::unordered_map<std::string, TypeId>();
	// Try to find the id of the component type in the map. 
	// If it doesn't exist then create it, else return the found value 
	auto it = m_stringTableId.find(_componentType);
	if (it == m_stringTableId.end())
	{
		TypeId newId = ++ComponentManagerData::NextTableId;
		m_stringTableId.insert(std::pair<std::string, TypeId>(_componentType, newId));
		return newId;
	}
	else
		return it->second;
}