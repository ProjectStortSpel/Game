#include "ComponentTypeReader.h"

using namespace ECSL;

ComponentTypeReader::ComponentTypeReader()
{

}

ComponentTypeReader::~ComponentTypeReader()
{

}

void ComponentTypeReader::ClearComponentType()
{
	m_name = "";
	m_tableType = TableType::None;
	m_variables = std::map<std::string, ComponentVariable>();
}

std::vector<ComponentType*>* ComponentTypeReader::ReadComponents(const std::string& _filePath, const Section* _section)
{
	m_componentTypes = new std::vector<ComponentType*>();
	ClearComponentType();

	/* No sections at all in file */
	if (_section->SubSections.size() == 0)
		return 0;

	/* Loop through and interpret every component type in the section list */
	for (unsigned int subSectionIndex = 0; subSectionIndex < _section->SubSections.size(); ++subSectionIndex)
	{
		if (!InterpretSubSection(&_section->SubSections[subSectionIndex], 1) || !ValidateComponentType())
		{
			printf("Invalid component type syntax in file: %s\n", _filePath.c_str());
			return 0;
		}
		m_componentTypes->push_back(new ComponentType(m_name, m_tableType, m_variables));
		ClearComponentType();
	}
	return m_componentTypes;
}

bool ComponentTypeReader::InterpretSubSection(const Section* _section, unsigned int _depth)
{
	/* First: Interpret the tokens*/
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

bool ComponentTypeReader::InterpretTokens(const Section* _section, const unsigned int _depth)
{
	/* If section is a new component type */
	if (_depth == 1)
		return InterpretComponentTypeName(_section, _depth);

	/* Else if section is inside a component type */
	else if (_depth == 2)
	{
		if (_section->Name == "Settings" && _depth == 2)
			return InterpretSettingTokens(_section, _depth);
		else if (_section->Name == "Data"  && _depth == 2)
			return InterpretDataTokens(_section, _depth);
	}

	return false;
}

bool ComponentTypeReader::InterpretComponentTypeName(const Section* _section, const unsigned int _depth)
{
	m_name = _section->Name;
	return true;
}

bool ComponentTypeReader::InterpretSettingTokens(const Section* _section, const unsigned int _depth)
{
	for (unsigned int tokenLineIndex = 0; tokenLineIndex < _section->Tokens.size(); ++tokenLineIndex)
	{
		std::vector<std::string> tokens = _section->Tokens[tokenLineIndex];
		std::string firstToken = tokens[0];
		if (firstToken == "Table")
		{
			if (tokens.size() != 2)
				return false;
			std::string secondToken = tokens[1];
			if (secondToken == "Array")
				m_tableType = TableType::Array;
			else if (secondToken == "Map")
				m_tableType = TableType::Map;
			else
				return false;
		}
	}
	return true;
}

bool ComponentTypeReader::InterpretDataTokens(const Section* _section, const unsigned int _depth)
{
	for (unsigned int tokenLineIndex = 0; tokenLineIndex < _section->Tokens.size(); ++tokenLineIndex)
	{
		std::vector<std::string> tokens = _section->Tokens[tokenLineIndex];
		std::string firstToken = tokens[0];
		std::string secondToken = tokens[1];
		if (tokens.size() != 2)
			return false;
		unsigned int byteSize = strtoul(tokens[1].c_str(), 0, 0);
		if (byteSize == 0)
		{
			std::string secondToken = tokens[1];
			if (secondToken == "float")
				byteSize = sizeof(float);
			else if (secondToken == "int")
				byteSize = sizeof(int);
			else if (secondToken == "pointer")
				byteSize = sizeof(void*);
			else
				return false;
		}

		ComponentVariable variable = ComponentVariable(firstToken, byteSize);
		m_variables.insert(std::pair<std::string, ComponentVariable>(variable.GetName(), variable));
	}
	return true;
}

bool ComponentTypeReader::ValidateComponentType()
{
	/* Component type must have variables and a table type */
	if (m_variables.size() == 0 || m_tableType == TableType::None)
	{
		return false;
	}
	return true;
}