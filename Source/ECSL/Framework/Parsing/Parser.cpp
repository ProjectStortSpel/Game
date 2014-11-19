#include "Parser.h"
#include <fstream>
#include <algorithm>
#include <string>

#include <stdio.h>

using namespace ECSL;

Parser::Parser()
{

}
Parser::~Parser()
{

}

void Parser::ParseFile(std::string _filename)
{
	std::ifstream fStream;
	fStream.open(_filename);

	if (!fStream)
		return;

	//	Read the file
	std::vector<std::string> fileRows = std::vector<std::string>();
	while (!fStream.eof())
	{
		std::string tText;
		fStream >> tText;

		if (tText.size() != 0)
			fileRows.push_back(tText);
	}

	//	Start parsing out the different parts
	ParsedComponent	tComponent = ParsedComponent();

	//	Parse the name for the component
	if (!ParseComponentName(tComponent, fileRows))
		printf("Something went wrong with 'ParseComponentName' method.\n");

	//	Parse the settings part
	if (!ParseComponentSettings(tComponent, fileRows))
		printf("Something went wrong with 'ParseComponentSettings' method.\n");


	fStream.close();
}

bool Parser::ParseComponentName(ParsedComponent& _Component, std::vector<std::string>& _FileRows)
{
	//	Check so syntax is followed. -> "Name"{}
	if (!CheckWrapSyntax(_FileRows))
		return false;

	//	Parse the name
	std::string	cName = ParseName(_FileRows[0]);

	//	See if name is correct
	if (cName.size() == 0)
		return false;

	//	Update the component with the new name and return
	_Component.Name = cName;

	//	Now remove the name and tags{} from the vector
	_FileRows.erase(_FileRows.begin());		//	"<Name>"
	_FileRows.erase(_FileRows.begin());		//	{
	_FileRows.erase(_FileRows.end() - 1);	//	}

	return true;
}
bool Parser::ParseComponentSettings(ParsedComponent& _Component, std::vector<std::string>& _FileRows)
{
	//	Check so syntax is followed. -> "Name"{}
	if (!CheckWrapSyntax(_FileRows))
		return false;

	//	Parse the name
	std::string	cName = ParseName(_FileRows[0]);

	//	See if name is correct
	if (cName.size() == 0)
		return false;

	//	Check spelling
	if (cName != "Settings")
		return false;

	//	Count number of settings
	int	nSettings = 0;
	for (int i = 2; i < _FileRows.size(); ++i)
	{
		if (_FileRows[i] == "}")
			break;

		++nSettings;
	}

	//	No settings
	if (nSettings == 0)
		return false;

	//	Start parsing settings entries
	for (int n = 0; n < nSettings; ++n)
	{
		std::string tEntry = _FileRows[2 + n];
		ParseComponentSettingsEntry(_Component, tEntry);
	}

	return true;
}

bool Parser::ParseComponentSettingsEntry(ParsedComponent& _Component, std::string _SettingsLine)
{

	return true;
}

std::string Parser::ParseName(std::string _text)
{
	//	Pick out the first entry, name for component
	std::string	firstValue = _text;

	//	Empty string where name is supposed to be
	if (firstValue.size() == 0)
		return "";

	//	Check if it has " symbols
	char	firstChar = firstValue[0];
	char	lastChar = firstValue[firstValue.size() - 1];

	//	Wrong syntax
	if (firstChar != '"' && lastChar != '"')
		return "";

	//	Remove the " symbols
	firstValue = firstValue.substr(1, firstValue.size() - 2);

	//	Name is empty -> ""
	if (firstValue.size() == 0)
		return "";

	return firstValue;
}

bool Parser::CheckWrapSyntax(std::vector<std::string>& _FileRows)
{
	//	No data available
	if (_FileRows.size() <= 3)
		return false;

	//	Check so each element is correct
	if (_FileRows[0].size() == 0 || _FileRows[1].size() == 0 || _FileRows[_FileRows.size() - 1].size() == 0)
		return false;

	//	Check syntax
	if (_FileRows[1][0] != '{' || _FileRows[_FileRows.size() - 1][0] != '}')
		return false;

	return true;
}