#include "Parser.h"

#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdio.h>
#include <cctype>
#include <stack>

using namespace ECSL;

Parser::Parser()
{

}

Parser::~Parser()
{

}

Section* Parser::ParseFile(const std::string& _filePath)
{
	std::ifstream file;
	file.open(_filePath);
	if (!file)
		return 0;

	std::vector<Line> lines;
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
		/* No point in adding empty lines */
		if (!IsLineEmpty(fileLine))
		{
			Line line;
			GetLineData(line, fileLine);
			lines.push_back(line);
		}
	}
	file.close();

	/* Check if the structure of the file is correct */
	if (!ValidateSyntax(lines))
	{
		printf("Invalid syntax in file: %s\n", _filePath.c_str());
		return 0;
	}

	std::vector<std::vector<std::string>> tokenizedLines;
	ConvertLinesToTokens(tokenizedLines, lines);

	Section* sectionTree = new Section();
	ConvertTokensToSections(sectionTree, tokenizedLines, lines);

	return sectionTree;
}

bool Parser::ValidateSymbols(const std::string& _fileLine)
{
	for (unsigned int symbolIndex = 0; symbolIndex < _fileLine.length(); ++symbolIndex)
	{
		char symbol = _fileLine[symbolIndex];
		if (GetSymbolType(symbol) == SymbolType::Invalid)
			return false;
	}
	return true;
}

void Parser::TrimLine(std::string& _fileLine)
{
	std::string trimmedString = "";
	for (unsigned int symbolIndex = 0; symbolIndex < _fileLine.length(); ++symbolIndex)
	{
		char symbol = _fileLine[symbolIndex];
		if (GetSymbolType(symbol) != SymbolType::EmptySpace)
			trimmedString.push_back(symbol);
	}
	_fileLine = trimmedString;
}

void Parser::GetLineData(Line& _line, const std::string& _fileLine)
{
	char firstSymbol = _fileLine[0];
	if (firstSymbol == DELIMITER_SYMBOL)
		_line.Type = LineType::Token;
	else if (firstSymbol == NEW_SECTION_SYMBOL)
		_line.Type = LineType::SectionStartBracket;
	else if (firstSymbol == END_SECTION_SYMBOL)
		_line.Type = LineType::SectionEndBracket;
	else
		_line.Type = LineType::None;

	for (unsigned int symbolIndex = 0; symbolIndex < _fileLine.length(); ++symbolIndex)
	{
		char symbol = _fileLine[symbolIndex];
		if (symbol == DELIMITER_SYMBOL)
			++_line.DelimiterSymbolCounter;
	}
	_line.Text = _fileLine;
}

bool Parser::ValidateSyntax(const std::vector<Line>& _lines)
{
	if (!ValidateLineSyntax(_lines))
		return false;
	if (!ValidateLineDependencies(_lines))
		return false;
	return true;
}

bool Parser::ValidateLineSyntax(const std::vector<Line>& _lines)
{
	/* Each line */
	for (unsigned int lineIndex = 0; lineIndex < _lines.size(); ++lineIndex)
	{
		Line line = _lines[lineIndex];
		bool insideToken = false;
		int symbolsCountInToken = 0;
		/* Each symbol in line */
		for (unsigned int symbolIndex = 0; symbolIndex < _lines[lineIndex].Text.length(); ++symbolIndex)
		{
			char symbol = _lines[lineIndex].Text[symbolIndex];
			SymbolType symbolType = GetSymbolType(symbol);
			switch (symbolType)
			{
			case SymbolType::Bracket:
				/* Generate an error if there already exists a token or a bracket on the line */
				if (line.Type != LineType::SectionStartBracket && line.Type != LineType::SectionEndBracket)
					return false;
				break;

			case SymbolType::TokenDelimiter:
				/* Generate an error if there already exists a bracket on the line */
				if (line.Type != LineType::Token)
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
				break;

			case SymbolType::Alphanumeric:
				/* Generate an error if the line isn't a token line */
				if (line.Type != LineType::Token)
					return false;

				/* Generate an error if the symbol isn't between the token delimiter symbols */
				if (!insideToken)
					return false;
				++symbolsCountInToken;
				break;

			default:
				break;
			}
		}

		/* Generate an error if there is an uneven number of token delimiter symbols */
		if (line.DelimiterSymbolCounter % 2 == 1)
			return false;
	}
	return true;
}

bool Parser::ValidateLineDependencies(const std::vector<Line>& _lines)
{
	int depth = 0;

	/* Each line */
	for (unsigned int lineIndex = 0; lineIndex < _lines.size(); ++lineIndex)
	{
		Line line = _lines[lineIndex];
		switch (line.Type)
		{
		case LineType::SectionStartBracket:
			/* Generate an error if there isn't a token line before the start bracket line */
			if (lineIndex == 0 || _lines[lineIndex - 1].Type != LineType::Token || _lines[lineIndex - 1].DelimiterSymbolCounter != 2)
				return false;
			++depth;
			break;
		case LineType::SectionEndBracket:
			--depth;
			break;
		case LineType::Token:
			/* Generate an error if a token line is the name of the section, but has more than two delimiter symbols (more than one token) */
			if (lineIndex == _lines.size() - 1 || (_lines[lineIndex + 1].Type == LineType::SectionStartBracket && line.DelimiterSymbolCounter != 2))
				return false;
			break;
		}
	}

	if (depth != 0)
		return false;

	return true;
}

void Parser::ConvertLinesToTokens(std::vector<std::vector<std::string>>& _tokenizedLines, const std::vector<Line>& _lines)
{
	/* Each line */
	for (unsigned int lineIndex = 0; lineIndex < _lines.size(); ++lineIndex)
	{
		_tokenizedLines.push_back(std::vector<std::string>());
		std::string token = "";
		/* Each symbol in line */
		for (unsigned int symbolIndex = 0; symbolIndex < _lines[lineIndex].Text.length(); ++symbolIndex)
		{
			char symbol = _lines[lineIndex].Text[symbolIndex];
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

void Parser::ConvertTokensToSections(Section* _sectionTree, const std::vector<std::vector<std::string>>& _tokenizedLines, const std::vector<Line>& _lines)
{
	Section* currentSection = _sectionTree;
	std::stack<Section*> sectionLevels;
	sectionLevels.push(currentSection);
	/* Loop through each line and add sections */
	for (unsigned int lineIndex = 0; lineIndex < _tokenizedLines.size(); ++lineIndex)
	{
		Section newSection;
		std::vector<std::string> tokenLine = _tokenizedLines[lineIndex];
		switch (_lines[lineIndex].Type)
		{
		case LineType::SectionStartBracket:
			newSection = Section();
			newSection.Name = _tokenizedLines[lineIndex - 1][0];
			(*currentSection).SubSections.push_back(newSection);
			sectionLevels.push(&((*currentSection).SubSections.back()));
			currentSection = sectionLevels.top();
			break;
		case LineType::SectionEndBracket:
			sectionLevels.pop();
			currentSection = sectionLevels.top();
			break;
		case LineType::Token:
			if (_lines[lineIndex + 1].Type != LineType::SectionStartBracket)
				AddTokens(currentSection, _tokenizedLines[lineIndex]);
			break;
		default:
			break;
		}
	}
}

void Parser::AddTokens(Section* _currentSection, const std::vector<std::string>& _tokens)
{
	/* Add a new token line */
	_currentSection->Tokens.push_back(std::vector<std::string>());
	/* Add the tokens to the line */
	for (auto token : _tokens)
	{
		_currentSection->Tokens.back().push_back(token);
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

bool Parser::IsLineEmpty(const std::string& _line)
{
	return (_line.length() == 0);
}

