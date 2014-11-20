#ifndef PARSER_H
#define PARSER_H

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <map>

#include "../Components/ComponentType.h"

namespace ECSL
{
	class DECLSPEC Parser
	{
	public:
		Parser();
		~Parser();

		void ParseFile(std::string _filename);
		ComponentType* ParseFilea(std::string _filename);

	private:
		const char m_delimiter = '"';

		std::map<std::string, int>* m_byteConversion;

		bool	ParseComponentName(ComponentType& _Component, std::vector<std::string>& _FileRows);

		bool	ParseSection(ComponentType& _Component, std::vector<std::string>& _FileRows, std::vector<std::string>& _SectionData);
		bool	ParseComponentSettingsEntry(ComponentType& _Component, std::string _SettingsLine);
		bool	ParseComponentDataEntry(ComponentType& _Component, std::string _SettingsLine);
		int		StringToByteSize(std::string _String);

		//Will check so the wrap for an element is correct.
		//I.e. has a name and {} surrounding it.
		bool	CheckWrapSyntax(std::vector<std::string>& _FileRows);

		//Strips the string from the banned symbols in the map
		std::string	StripString(std::string _text, std::map<char, bool>& _bannedSymbols);

		//Will parse the name of a given string, removing " symbols
		std::string ParseName(std::string _text);



	};
}

#endif