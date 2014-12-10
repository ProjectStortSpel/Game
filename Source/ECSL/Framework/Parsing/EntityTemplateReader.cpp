#include "EntityTemplateReader.h"

using namespace ECSL;

EntityTemplateReader::EntityTemplateReader()
{

}

EntityTemplateReader::~EntityTemplateReader()
{

}

void EntityTemplateReader::ClearEntityTemplate()
{
	m_name = "";
	m_components = std::map<std::string, std::vector<TemplateEntry>>();
}

bool EntityTemplateReader::ReadTemplates(std::vector<EntityTemplate*>& _out, const std::string& _filePath, const Section& _section)
{
	ClearEntityTemplate();

	/* No sections at all in file */
	if (_section.SubSections.size() == 0)
		return false;

	/* Loop through and interpret every component type in the section list */
	for (unsigned int subSectionIndex = 0; subSectionIndex < _section.SubSections.size(); ++subSectionIndex)
	{
		if (!InterpretSubSection(&_section.SubSections[subSectionIndex], 1) || !ValidateEntityTemplate())
		{
			printf("Invalid component type syntax in file: %s\n", _filePath.c_str());
			return false;
		}
		_out.push_back(new EntityTemplate(m_name, m_components));
		ClearEntityTemplate();
	}

	return true;
}

bool EntityTemplateReader::InterpretSubSection(const Section* _section, unsigned int _depth)
{
	/* First: Interpret the tokens */
	if (!InterpretTokens(_section, _depth))
		return false;

	/* Second: Jump to a deeper section */
	for (unsigned int subSectionIndex = 0; subSectionIndex < _section->SubSections.size(); ++subSectionIndex)
	{
		if (!InterpretSubSection(&_section->SubSections[subSectionIndex], _depth + 1))
			return false;
	}
	return true;
}

bool EntityTemplateReader::InterpretTokens(const Section* _section, const unsigned int _depth)
{
	/* If section is a new component type */
	if (_depth == 1)
		return InterpretTemplateName(_section, _depth);

	/* Else if section is inside a component type */
	else if (_depth == 2)
	{
		if (_section->Name == "Components" && _depth == 2)
			return InterpretComponentTokens(_section, _depth);
		else if (_section->Name == "Data"  && _depth == 2)
			return InterpretDataTokens(_section, _depth);
	}

	return false;
}

bool EntityTemplateReader::InterpretTemplateName(const Section* _section, const unsigned int _depth)
{
	m_name = _section->Name;
	return true;
}

bool EntityTemplateReader::InterpretComponentTokens(const Section* _section, const unsigned int _depth)
{
	for (unsigned int tokenLineIndex = 0; tokenLineIndex < _section->Tokens.size(); ++tokenLineIndex)
	{
		std::vector<std::string> tokens = _section->Tokens[tokenLineIndex];
		std::string firstToken = tokens[0];

		m_components.insert(std::pair<std::string, std::vector<TemplateEntry>>(firstToken, std::vector<TemplateEntry>()));
	}
	return true;
}

bool EntityTemplateReader::InterpretDataTokens(const Section* _section, const unsigned int _depth)
{
	for (unsigned int tokenLineIndex = 0; tokenLineIndex < _section->Tokens.size(); ++tokenLineIndex)
	{
		std::vector<std::string> tokens = _section->Tokens[tokenLineIndex];
		std::string componentType = tokens[0];

		for (int nData = 1; nData < tokens.size(); ++nData)
		{
			if (!ConvertDataEntry(componentType, tokens[nData]))
			{
				printf("Could not convert data correct for \"%s\" (\"%s\" unvalid!)\n", componentType, tokens[nData]);
				return 0;
			}
		}
	}
	return true;
}

bool EntityTemplateReader::ConvertDataEntry(std::string& _componentType, std::string& _dataEntry)
{
	/*	Determine if the token is a number or text	*/
	bool isText = true;
	for (int n = 0; n < _dataEntry.size(); ++n)
		if (!isalpha(_dataEntry[n]))
		{
			isText = false;
			break;
		}
			
	if (isText)
	{
		m_components[_componentType].push_back(TemplateEntry(_dataEntry));
	}
	else
	{
		/*	Is the number a integer or float	*/
		if (tolower(_dataEntry[_dataEntry.size() - 1]) == 'f')
			m_components[_componentType].push_back((float)atof(_dataEntry.c_str()));
		else
			m_components[_componentType].push_back(atoi(_dataEntry.c_str()));
	}

	return true;
}

bool EntityTemplateReader::ValidateEntityTemplate()
{
	/*	Check if valid data was read	*/
	if (m_name.size() == 0 || m_components.size() == 0)
		return false;

	return true;
}