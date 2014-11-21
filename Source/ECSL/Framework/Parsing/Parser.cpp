#include "Parser.h"

#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdio.h>
#include <cctype>

using namespace ECSL;

Parser::Parser()
{
	//	Add more symbols
	m_byteConversion = new std::map<std::string, int>();
	//m_byteConversion->insert(std::pair<std::string, int>("float", sizeof(float)));
	//m_byteConversion->at("float") = sizeof(float);
	//m_byteConversion->at("int") = sizeof(int);
	//m_byteConversion->at("double") = sizeof(double);
	//m_byteConversion->at("pointer") = sizeof(void*);
}

Parser::~Parser()
{

}

std::vector<Section>* Parser::ParseFile(const std::string& _filePath)
{
	std::ifstream file;
	file.open(_filePath);
	if (!file)
		return 0;

	std::vector<std::string> trimmedLines;
	std::string fileLine;
	/* Loop through each line in file. Validate symbols and trim the line */
	for (int i = 0; std::getline(file, fileLine); ++i)
	{
		if (!ValidateSymbols(fileLine))
		{
			printf("Invalid symbols used in file: %s\nLine: %i\n", _filePath.c_str(), i + 1);
			return 0;
		}
		TrimLine(fileLine);
		if (!IsLineEmpty(fileLine))
			trimmedLines.push_back(fileLine);
	}
	file.close();

	/* Check if the structure of the file is correct */
	if (!ValidateTokenStructure(trimmedLines))
	{
		printf("Invalid text structure in file: %s\n", _filePath.c_str());
		return 0;
	}

	std::vector<std::vector<std::string>> tokenizedLines;
	ConvertLinesToTokens(tokenizedLines, trimmedLines);

	std::vector<Section>* sections = new std::vector<Section>();
	if (!TokensToSections(*sections, tokenizedLines))
	{
		printf("Error when parsing file: %s", _filePath.c_str());
		return 0;
	}

	return sections;
}

bool Parser::TokensToSections(std::vector<Section>& _sections, const std::vector<std::vector<std::string>>& _tokenizedLines)
{
	//for (int i = 0; i < _tokenizedLines.size(); ++i)
	//{
	//	std::string fileLine = _tokenizedLines.at(i);
	//	std::vector<std::string> tokens;
	//	GetTokens(tokens, fileLine);
	//	
	//}
	return false;
}

bool Parser::ValidateSymbols(const std::string& _line)
{
	for (int i = 0; i < _line.length(); ++i)
	{
		char symbol = _line[i];
		if (GetSymbolType(symbol) == SymbolType::Invalid)
			return false;
	}
	return true;
}

bool Parser::ValidateTokenStructure(const std::vector<std::string>& _trimmedLines)
{
	std::vector<Parser::Line> lines;
	int indentationCounter = 0;

	/* Each line */
	for (int i = 0; i < _trimmedLines.size(); ++i)
	{
		Parser::Line line;
		bool insideToken = false;
		int symbolsCountInToken = 0;
		/* Each symbol in line */
		for (int n = 0; n < _trimmedLines[i].length(); ++n)
		{
			char symbol = _trimmedLines[i][n];
			SymbolType symbolType = GetSymbolType(symbol);
			/* Symbol is a bracket */
			if (symbolType == SymbolType::Bracket)
			{
				/* Generate an error if there already exists a token or a bracket on the line */
				if (line.Type == Line::LineType::Token || line.Type == Line::LineType::SectionStartBracket || line.Type == Line::LineType::SectionEndBracket)
					return false;
				
				/* Start section bracket */
				if (symbol == NEW_SECTION_SYMBOL)
				{
					++indentationCounter;
					line.Type = Line::LineType::SectionStartBracket;
				}
				/* End section bracket  */
				else if (symbol == END_SECTION_SYMBOL)
				{
					--indentationCounter;
					line.Type = Line::LineType::SectionEndBracket;
				}
				
			}
			/* Symbol is a token delimiter symbol */
			else if (symbolType == SymbolType::TokenDelimiter)
			{
				/* Generate an error if there already exists a bracket on the line */
				if (line.Type == Line::LineType::SectionStartBracket || line.Type == Line::LineType::SectionEndBracket)
					return false;

				/* Start of a token */
				if (!insideToken)
				{
					insideToken = true;
					symbolsCountInToken = 0;
				}
				/* End of a token */
				else if (insideToken)
				{
					insideToken = false;
					/* Generate an error if the number of symbols inside the token is zero */
					if (symbolsCountInToken == 0)
						return false;
				}
				line.Type = Line::Token;
				++line.TokenSymbolCounter;
			}
			/* Symbol is an alphanumeric symbol */
			else if (symbolType == SymbolType::Alphanumeric)
			{
				/* Generate an error if the symbol isn't between the token delimiter symbols */
				if (!insideToken)
					return false;
				++symbolsCountInToken;
			}
		}

		/* Generate an error if there are an uneven number of token delimiter symbols */
		if (line.TokenSymbolCounter % 2 == 1)
			return false;

		/* If the line is a bracket line, then it needs to have a token in the earlier name (The name of the section)*/
		if (line.Type == Line::LineType::SectionStartBracket)
		{
			/* Out of index */
			if (i == 0)
				return false;
			/* If line before it needs to be a token line */
			else if (lines[i - 1].Type != Line::LineType::Token)
				return false;
			/* The number of tokens in the line before it needs to be one */
			else if (lines[i - 1].TokenSymbolCounter != 2)
				return false;
		}

		lines.push_back(line);
	}

	if (indentationCounter != 0)
		return false;

	return true;
}

bool Parser::IsLineEmpty(const std::string& _line)
{
	return (_line.length() == 0);
}

void Parser::TrimLine(std::string& _line)
{
	std::string trimmedString = "";
	for (int i = 0; i < _line.length(); ++i)
	{
		char symbol = _line[i];
		/* Remove all empty space */
		if (GetSymbolType(symbol) != SymbolType::EmptySpace)
			trimmedString.push_back(symbol);
	}
	_line = trimmedString;
}

void Parser::ConvertLinesToTokens(std::vector<std::vector<std::string>>& _tokenizedLines, const std::vector<std::string>& _trimmedLines)
{
	for (int lineIndex = 0; lineIndex < _trimmedLines.size(); ++lineIndex)
	{
		_tokenizedLines.push_back(std::vector<std::string>());
		bool insideToken = false;
		std::string token = "";
		
		for (int symbolIndex = 0; symbolIndex < _trimmedLines[lineIndex].length(); ++symbolIndex)
		{
			char symbol = _trimmedLines[lineIndex][symbolIndex];
			SymbolType symbolType = GetSymbolType(symbol);

			switch (symbolType)
			{
			case SymbolType::Bracket:
				_tokenizedLines[lineIndex].push_back(std::string(1, symbol));
				break;

			case SymbolType::TokenDelimiter:
				if (token != "")
				{
					_tokenizedLines[lineIndex].push_back(token);
					token = "";
				}
				break;

			case SymbolType::Alphanumeric:
				token.push_back(symbol);
				break;
		
			default:
				break;
			}
		}
	}
}

Parser::SymbolType Parser::GetSymbolType(char _symbol)
{
	if (_symbol == NEW_SECTION_SYMBOL || _symbol == END_SECTION_SYMBOL)
		return SymbolType::Bracket;
	else if (_symbol == DELIMITER_SYMBOL)
		return SymbolType::TokenDelimiter;
	else if (std::isalnum(_symbol))
		return SymbolType::Alphanumeric;
	else if (_symbol == '\t' || _symbol == ' ')
		return SymbolType::EmptySpace;
	return SymbolType::Invalid;
}