#ifndef ENTITYTEMPLATEMANAGER_H
#define ENTITYTEMPLATEMANAGER_H

#include <SDL/SDL.h>
#include <string>
#include <unordered_map>
#include <map>

#include "../Framework/Common/FileHelper.h"
#include "../Framework/Templates/EntityTemplate.h"
#include "../Framework/Parsing/EntityTemplateReader.h"

namespace ECSL
{
	class DECLSPEC EntityTemplateManager
	{
	public:
		~EntityTemplateManager();
		static EntityTemplateManager& GetInstance();

		void LoadComponentTypesFromDirectory(const std::string& _directoryPath);
		void LoadComponentTypesFromFile(const std::string& _filePath);

		EntityTemplate* GetTemplate(unsigned int _templateId);
		EntityTemplate* GetTemplate(std::string _template);

		unsigned int GetTemplateCount(){ return (unsigned int)m_entityTemplates->size(); }

		unsigned int GetTableId(const std::string& _templateName);

		void AddTemplate(EntityTemplate* _template);

		bool HasTemplate(std::string& _template);
		
		void Clear();
	private:
		EntityTemplateManager();

		int m_nextTableId;
		Parser* m_parser;
		EntityTemplateReader* m_entityTemplateReader;
		std::map<unsigned int, EntityTemplate*>* m_entityTemplates;
		std::unordered_map<std::string, unsigned int>* m_stringTableId;
	};

}

#endif