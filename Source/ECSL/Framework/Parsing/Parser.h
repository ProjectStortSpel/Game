#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include <map>

namespace ECSL
{
	struct SettingsStruct
	{
		std::string	Name;
		std::string	Value;
	};
	struct DataStruct
	{
		std::string	Name;
		int ByteSize;
	};

	struct ParsedComponent
	{
		std::string	Name;
		std::vector<SettingsStruct> Settings;
		std::vector<DataStruct> Data;
		std::vector<std::string> DebugStrings;
	};

	class Parser
	{
	public:
		Parser();
		~Parser();

		void ParseFile(std::string _filename);

	private:
		bool	ParseComponentName(ParsedComponent& _Component, std::vector<std::string>& _FileRows);

		bool	ParseSection(ParsedComponent& _Component, std::vector<std::string>& _FileRows, std::vector<std::string>& _SectionData);
		bool	ParseComponentSettingsEntry(ParsedComponent& _Component, std::string _SettingsLine);
		bool	ParseComponentDataEntry(ParsedComponent& _Component, std::string _SettingsLine);
		int		StringToByteSize(std::string _String);

		//Will check so the wrap for an element is correct.
		//I.e. has a name and {} surrounding it.
		bool	CheckWrapSyntax(std::vector<std::string>& _FileRows);

		//Strips the string from the banned symbols in the map
		std::string	StripString(std::string _text, std::map<char, bool>& _bannedSymbols);

		//Will parse the name of a given string, removing " symbols
		std::string ParseName(std::string _text);


		std::map<std::string, int> m_byteConversion;
	};
}

#endif