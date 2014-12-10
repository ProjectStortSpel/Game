#ifndef ENTITYTEMPLATEREADER_H
#define ENTITYTEMPLATEREADER_H

#include <SDL/SDL.h>
#include <vector>

#include "Parser.h"
#include "../Templates/EntityTemplate.h"

namespace ECSL
{
	class DECLSPEC EntityTemplateReader
	{
	private:
		std::string m_name;
		std::map<std::string, std::vector<TemplateEntry>> m_components;

		bool InterpretSubSection(const Section* _section, unsigned int _depth);
		bool InterpretTokens(const Section* _section, const unsigned int _depth);

		bool InterpretTemplateName(const Section* _section, const unsigned int _depth);
		bool InterpretComponentTokens(const Section* _section, const unsigned int _depth);
		bool InterpretDataTokens(const Section* _section, const unsigned int _depth);

		bool ConvertDataEntry(std::string& _componentType, std::string& _dataEntry);

		bool ValidateEntityTemplate();

		void ClearEntityTemplate();
	public:
		EntityTemplateReader();
		~EntityTemplateReader();

		bool ReadTemplates(std::vector<EntityTemplate*>& _out, const std::string& _filePath, const Section& _section);
	};
}

#endif