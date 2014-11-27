#include "ComponentTypeManager.h"

#include "../Framework/Common/FileHelper.h"

using namespace ECSL;

ComponentTypeManager& ComponentTypeManager::GetInstance()
{
	static ComponentTypeManager* instance = new ComponentTypeManager();
	return *instance;
}

ComponentTypeManager::ComponentTypeManager()
: m_componentTypes(new std::map<int, ComponentType*>()), m_parser(new Parser()), m_componentTypeReader(new ComponentTypeReader())
{
}

void ComponentTypeManager::AddComponentType(ComponentType& _componentType)
{
	m_componentTypes->insert(std::pair<int, ComponentType*>(ComponentTypeManager::GetTableId(_componentType.GetName()), &_componentType));
}

void ComponentTypeManager::LoadComponentTypesFromDirectory(const std::string& _directoryPath)
{
	std::vector<std::string> filePaths;
	/* Get all component type file paths in the specified directory */
	FileHelper::GetFilesInDirectory(filePaths, _directoryPath, "cmp");
	/* Load every component type from every file */
	for (unsigned int i = 0; i < filePaths.size(); ++i)
		LoadComponentTypesFromFile(filePaths[i]);
}

void ComponentTypeManager::LoadComponentTypesFromFile(const std::string& _filePath)
{
	Section section;
	/* Parse the file into an interpretable section tree */
	if (!m_parser->ParseFile(section, _filePath))
		return;
	
	std::vector<ComponentType*> components;
	/* Interpret the section tree into a component type */
	if (!m_componentTypeReader->ReadComponents(components, _filePath, section))
		return;

	/* Add all component types that was read from the file */
	for (unsigned int i = 0; i < components.size(); ++i)
		AddComponentType(*components[i]);
}

ComponentType* ComponentTypeManager::GetComponentType(int _componentTypeId)
{
	auto it = m_componentTypes->find(_componentTypeId);
	if (it != m_componentTypes->end())
		return it->second;
	return 0;
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