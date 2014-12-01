#ifndef COMPONENTTYPEREADER_H
#define COMPONENTTYPEREADER_H

#include <SDL/SDL.h>
#include <vector>

#include "Parser.h"
#include "../Components/ComponentType.h"

namespace ECSL
{
	class ComponentTypeReader
	{
	private:
		std::string m_name;
		TableType m_tableType;
		std::map<std::string, ComponentVariable> m_variables;

		bool InterpretSubSection(const Section* _section, unsigned int _depth);
		bool InterpretTokens(const Section* _section, const unsigned int _depth);
		bool InterpretComponentTypeName(const Section* _section, const unsigned int _depth);
		bool InterpretSettingTokens(const Section* _section, const unsigned int _depth);
		bool InterpretDataTokens(const Section* _section, const unsigned int _depth);
		bool ValidateComponentType();

		void ClearComponentType();
	public:
		ComponentTypeReader();
		~ComponentTypeReader();

		bool ReadComponents(std::vector<ComponentType*>& _out, const std::string& _filePath, const Section& _section);
	};
}

#endif