#include "EntityTemplateManager.h"
using namespace ECSL;

EntityTemplateManager::EntityTemplateManager() : 
	m_entityTemplates(new std::map<unsigned int, EntityTemplate*>()),
	m_parser(new Parser()), m_entityTemplateReader(new EntityTemplateReader()),
	m_stringTableId(new std::unordered_map<std::string, unsigned int>()),
	m_nextTableId(-1)
{

}

EntityTemplateManager::~EntityTemplateManager()
{
	if (m_stringTableId)
	{
		m_stringTableId->clear();
		delete(m_stringTableId);
	}

	if (m_entityTemplates)
	{
		for (auto it = m_entityTemplates->begin(); it != m_entityTemplates->end(); ++it)
			delete it->second;

		m_entityTemplates->clear();
		delete m_entityTemplates;
	}

	if (m_entityTemplateReader)
		delete m_entityTemplateReader;

	if (m_parser)
		delete m_parser;
}

EntityTemplateManager& EntityTemplateManager::GetInstance()
{
	static EntityTemplateManager* instance = new EntityTemplateManager();
	return *instance;
}

void EntityTemplateManager::LoadComponentTypesFromDirectory(const std::string& _directoryPath)
{
	std::vector<std::string> filePaths;
	/* Get all component type file paths in the specified directory */
	FileHelper::GetFilesInDirectory(filePaths, _directoryPath, "tmp");
	/* Load every component type from every file */
	for (unsigned int i = 0; i < filePaths.size(); ++i)
		LoadComponentTypesFromFile(filePaths[i]);
}
void EntityTemplateManager::LoadComponentTypesFromFile(const std::string& _filePath)
{
	Section section;
	/* Parse the file into an interpretable section tree */
	if (!m_parser->ParseFile(section, _filePath))
		return;

	std::vector<EntityTemplate*> templates;
	/* Interpret the section tree into a component type */
	if (!m_entityTemplateReader->ReadTemplates(templates, _filePath, section))
		return;

	/* Add all component types that was read from the file */
	for (unsigned int i = 0; i < templates.size(); ++i)
		AddTemplate(*templates[i]);
}

EntityTemplate* EntityTemplateManager::GetTemplate(std::string _template)
{
	return GetTemplate(GetTableId(_template));
}
EntityTemplate* EntityTemplateManager::GetTemplate(unsigned int _templateId)
{
	auto it = m_entityTemplates->find(_templateId);
	if (it != m_entityTemplates->end())
		return it->second;
	return 0;
}

void EntityTemplateManager::AddTemplate(EntityTemplate& _template)
{
	m_entityTemplates->insert(std::pair<int, EntityTemplate*>(EntityTemplateManager::GetTableId(_template.GetName()), &_template));
}

unsigned int EntityTemplateManager::GetTableId(const std::string& _templateName)
{
	// Try to find the id of the component type in the map. 
	// If it doesn't exist then create it, else return the found value 
	auto it = m_stringTableId->find(_templateName);
	if (it == m_stringTableId->end())
	{
		unsigned int newId = ++m_nextTableId;
		m_stringTableId->insert(std::pair<std::string, unsigned int>(_templateName, newId));
		return newId;
	}
	else
		return it->second;
}



bool EntityTemplateManager::HasTemplate(std::string& _template)
{
void EntityTemplateManager::Clear()
	return m_stringTableId->find(_template) != m_stringTableId->end();
}
		m_stringTableId->clear();
		delete(m_stringTableId);
	}

	if (m_entityTemplates)
	{
		for (auto it = m_entityTemplates->begin(); it != m_entityTemplates->end(); ++it)
			delete it->second;

		m_entityTemplates->clear();
		delete m_entityTemplates;
	}

	if (m_entityTemplateReader)
		delete m_entityTemplateReader;

	if (m_parser)
		delete m_parser;
	
	m_nextTableId = -1;
	m_parser = new Parser();
	m_entityTemplateReader = new EntityTemplateReader();
	m_entityTemplates = new std::map<unsigned int, EntityTemplate*>();
	m_stringTableId = new std::unordered_map<std::string, unsigned int>();
}