#include "ComponentTypeManager.h"
#include <assert.h>

#include "../Framework/Common/FileHelper.h"

using namespace ECSL;

ComponentTypeManager& ComponentTypeManager::GetInstance()
{
	static ComponentTypeManager* instance = new ComponentTypeManager();
	return *instance;
}

ComponentTypeManager::ComponentTypeManager()
:	m_componentTypes(new std::map<unsigned int, ComponentType*>()), 
	m_parser(new Parser()), m_componentTypeReader(new ComponentTypeReader()), 
	m_stringTableId(new std::unordered_map<std::string, unsigned int>()),
	m_nextTableId(-1)
{
}

ComponentTypeManager::~ComponentTypeManager()
{
	if (m_stringTableId)
	{
		m_stringTableId->clear();
		delete(m_stringTableId);
	}

	if (m_componentTypes)
	{
		for (auto it = m_componentTypes->begin(); it != m_componentTypes->end(); ++it)
			delete it->second;

		m_componentTypes->clear();
		delete m_componentTypes;
	}

	if (m_componentTypeReader)
		delete m_componentTypeReader;

	if (m_parser)
		delete m_parser;

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

ComponentType* ComponentTypeManager::GetComponentType(unsigned int _componentTypeId)
{
	auto it = m_componentTypes->find(_componentTypeId);
	if (it != m_componentTypes->end())
		return it->second;
	return 0;
}

unsigned int ComponentTypeManager::GetTableId(const std::string& _componentType)
{
	// Try to find the id of the component type in the map. 
	// If it doesn't exist then create it, else return the found value 
	auto it = m_stringTableId->find(_componentType);
	if (it == m_stringTableId->end())
	{
		unsigned int newId = ++m_nextTableId;

		m_stringTableId->insert(std::pair<std::string, unsigned int>(_componentType, newId));
		return newId;
	}
	else
		return it->second;
}

void ComponentTypeManager::AddComponentType(ComponentType& _componentType)
{
	/*	Trying to add a component that is already defined	*/
	assert(m_stringTableId->find(_componentType.GetName()) == m_stringTableId->end());

	m_componentTypes->insert(std::pair<int, ComponentType*>(ComponentTypeManager::GetTableId(_componentType.GetName()), &_componentType));
}

bool ComponentTypeManager::ComponentExists(std::string& _componentType)
{
	return m_stringTableId->find(_componentType) != m_stringTableId->end();
}

void ComponentTypeManager::Clear()
{
	if (m_stringTableId)
	{
		m_stringTableId->clear();
		delete(m_stringTableId);
	}

	if (m_componentTypes)
	{
		for (auto it = m_componentTypes->begin(); it != m_componentTypes->end(); ++it)
			delete it->second;

		m_componentTypes->clear();
		delete m_componentTypes;
	}

	if (m_componentTypeReader)
		delete m_componentTypeReader;

	if (m_parser)
		delete m_parser;
	
	m_nextTableId = -1;
	m_parser = new Parser();
	m_componentTypeReader = new ComponentTypeReader();
	m_componentTypes = new std::map<unsigned int, ComponentType*>();
	m_stringTableId = new std::unordered_map<std::string, unsigned int>();
}