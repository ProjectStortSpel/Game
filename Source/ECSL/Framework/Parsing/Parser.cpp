#include "Parser.h"
#include <fstream>
#include <algorithm>
#include <string>

#include <stdio.h>

using namespace ECSL;

Parser::Parser()
{
	//	Add more symbols
	m_byteConversion = std::map<std::string, int>();
	m_byteConversion["float"] = sizeof(float);
	m_byteConversion["int"] = sizeof(int);
	m_byteConversion["double"] = sizeof(double);
	m_byteConversion["pointer"] = sizeof(nullptr);
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

	//	Create banned symbols
	std::map<char, bool> bannedSymbols = std::map<char, bool>();
	bannedSymbols['\t'] = true;
	bannedSymbols[' '] = true;

	//	Read the file
	std::vector<std::string> fileRows = std::vector<std::string>();
	std::string tLine;
	while (std::getline(fStream, tLine))
	{
		tLine = StripString(tLine, bannedSymbols);

		if (tLine.size() != 0)
			fileRows.push_back(tLine);
	}
	fStream.close();

	//	Start parsing out the different parts
	ParsedComponent	tComponent = ParsedComponent();
	std::vector<std::string> stringVector = std::vector<std::string>();

	//	Parse the name for the component
	if (!ParseComponentName(tComponent, fileRows))
	{
		printf("Something went wrong with 'ParseComponentName' method.\n");
		return;
	}
	//	Parse the settings part
	if (!ParseSection(tComponent, fileRows, stringVector) || stringVector.size() == 0)
	{
		printf("Something went wrong with 'ParseSection'(SETTINGS) method.\n");
		return;
	}

	//	Parse the data entries
	for (int n = 0; n < stringVector.size(); ++n)
		ParseComponentSettingsEntry(tComponent, stringVector[n]);
	stringVector.clear();

	//	Parse the data part
	if (!ParseSection(tComponent, fileRows, stringVector) || stringVector.size() == 0)
	{
		printf("Something went wrong with 'ParseSection'(DATA) method.\n");
		return;
	}

	//	Parse the data entries
	for (int n = 0; n < stringVector.size(); ++n)
		ParseComponentDataEntry(tComponent, stringVector[n]);
	stringVector.clear();

	//	Parse the debug part
	if (!ParseSection(tComponent, fileRows, stringVector) || stringVector.size() == 0)
	{
		printf("Something went wrong with 'ParseSection'(DEBUG) method.\n");
		return;
	}
	//	Parse the data entries
	for (int n = 0; n < stringVector.size(); ++n)
		tComponent.DebugStrings.push_back(stringVector[n]);
	stringVector.clear();
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
bool Parser::ParseSection(ParsedComponent& _Component, std::vector<std::string>& _FileRows, std::vector<std::string>& _SectionData)
{
	//	Check so syntax is followed. -> "Name"{}
	if (!CheckWrapSyntax(_FileRows))
		return false;

	//	Parse the name
	std::string	cName = ParseName(_FileRows[0]);

	//	See if name is correct
	if (cName.size() == 0)
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

	//	Pop the two first rows -> Name and {
	_FileRows.erase(_FileRows.begin());
	_FileRows.erase(_FileRows.begin());

	//	Start parsing settings entries
	for (int n = 0; n < nSettings; ++n)
	{
		std::string tEntry = _FileRows[0];
		_SectionData.push_back(tEntry);

		//	Pop that row from the read lines
		_FileRows.erase(_FileRows.begin());
	}
	//	Pop the last }
	_FileRows.erase(_FileRows.begin());

	return true;
}

bool Parser::ParseComponentSettingsEntry(ParsedComponent& _Component, std::string _SettingsLine)
{
	//	Split the string
	int separateIndex[2] = { -1, -1 };
	std::vector<std::string> splitEntries = std::vector<std::string>();
	for (int i = _SettingsLine.size() - 1; i >= 0; --i)
	{
		char tChar = _SettingsLine[i];
		if (tChar == '"')
		{
			if (separateIndex[1] == -1)
				separateIndex[1] = i;
			else if (separateIndex[0] == -1)
				separateIndex[0] = i;

			if (separateIndex[0] != -1 && separateIndex[1] != -1)
			{
				splitEntries.push_back(_SettingsLine.substr(separateIndex[0] + 1, (separateIndex[1] - separateIndex[0]) - 1));
				separateIndex[0] = -1;
				separateIndex[1] = -1;
			}
		}


	}
	//	Not enough data
	if (splitEntries.size() <= 1)
		return false;

	std::string	tField = splitEntries[splitEntries.size() - 1];
	std::string tValue = splitEntries[0];

	SettingsStruct	newEntry = SettingsStruct();
	newEntry.Name = tField;
	newEntry.Value = tValue;

	_Component.Settings.push_back(newEntry);


	return true;
}

bool Parser::ParseComponentDataEntry(ParsedComponent& _Component, std::string _SettingsLine)
{
	//	Split the string
	int separateIndex[2] = { -1, -1 };
	std::vector<std::string> splitEntries = std::vector<std::string>();
	for (int i = _SettingsLine.size() - 1; i >= 0; --i)
	{
		char tChar = _SettingsLine[i];
		if (tChar == '"')
		{
			if (separateIndex[1] == -1)
				separateIndex[1] = i;
			else if (separateIndex[0] == -1)
				separateIndex[0] = i;

			if (separateIndex[0] != -1 && separateIndex[1] != -1)
			{
				splitEntries.push_back(_SettingsLine.substr(separateIndex[0] + 1, (separateIndex[1] - separateIndex[0]) - 1));
				separateIndex[0] = -1;
				separateIndex[1] = -1;
			}
		}


	}
	//	Not enough data
	if (splitEntries.size() <= 1)
		return false;

	//	Pick out the right parts of the string
	std::string	tField = splitEntries[splitEntries.size() - 1];
	std::string tValue = splitEntries.size() == 2 ? splitEntries[0] : splitEntries[1];
	int tElements = splitEntries.size() == 2 ? 1 : atoi(splitEntries[0].c_str());

	//	Check so number of elements is a positive number
	if (tElements <= 0)
		return false;

	DataStruct	newEntry = DataStruct();
	newEntry.Name = tField;
	int isNumber = atoi(tValue.c_str());
	if (isNumber == 0)
	{
		if (m_byteConversion.find(tValue) == m_byteConversion.end())
			return false;
		
		newEntry.ByteSize = m_byteConversion[tValue];
	}
	else
		newEntry.ByteSize = isNumber;

	newEntry.ByteSize *= tElements;

	_Component.Data.push_back(newEntry);


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

std::string Parser::StripString(std::string _text, std::map<char, bool>& _bannedSymbols)
{
	for (int i = _text.size() - 1; i >= 0; --i)
	{
		char tChar = _text[i];

		if (_bannedSymbols.find(tChar) != _bannedSymbols.end())
			_text.erase(_text.begin() + i);

	}

	return _text;
}