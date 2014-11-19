#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
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
	};

	class Parser
	{
	public:
		Parser();
		~Parser();

		void ParseFile(std::string _filename);

	private:
		bool	ParseComponentName(ParsedComponent& _Component, std::vector<std::string>& _FileRows);

		bool	ParseComponentSettings(ParsedComponent& _Component, std::vector<std::string>& _FileRows);
		bool	ParseComponentSettingsEntry(ParsedComponent& _Component, std::string _SettingsLine);

		//Will check so the wrap for an element is correct.
		//I.e. has a name and {} surrounding it.
		bool	CheckWrapSyntax(std::vector<std::string>& _FileRows);

		//Will parse the name of a given string, removing " symbols
		std::string ParseName(std::string _text);

		void ParseDataSection();

	};
}

#endif