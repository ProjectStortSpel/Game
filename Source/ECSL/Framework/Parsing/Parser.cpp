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

Section* Parser::ParseFile(const std::string& _filePath)
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
		/* No point in adding empty lines */
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

	Section* sectionTree = new Section();
	ConvertTokensToSections(sectionTree, tokenizedLines);

	return sectionTree;
}

bool Parser::ValidateSymbols(const std::string& _line)
{
	for (unsigned int i = 0; i < _line.length(); ++i)
	{
		char symbol = _line[i];
		if (GetSymbolType(symbol) == SymbolType::Invalid)
			return false;
	}
	return true;
}

void Parser::TrimLine(std::string& _line)
{
	std::string trimmedString = "";
	for (unsigned int i = 0; i < _line.length(); ++i)
	{
		char symbol = _line[i];
		/* Remove all empty space */
		if (GetSymbolType(symbol) != SymbolType::EmptySpace)
			trimmedString.push_back(symbol);
	}
	_line = trimmedString;
}

//bool Parser::ValidateTokenStructure(const std::vector<std::string>& _trimmedLines)
//{
//	std::vector<Parser::Line> lines;
//	int indentationCounter = 0;
//
//	/* Each line */
//	for (unsigned int lineIndex = 0; lineIndex < _trimmedLines.size(); ++lineIndex)
//	{
//		Parser::Line line;
//		bool insideToken = false;
//		int symbolsCountInToken = 0;
//		/* Each symbol in line */
//		for (unsigned int symbolIndex = 0; symbolIndex < _trimmedLines[lineIndex].length(); ++symbolIndex)
//		{
//			char symbol = _trimmedLines[lineIndex][symbolIndex];
//			SymbolType symbolType = GetSymbolType(symbol);
//			/* Symbol is a bracket */
//			if (symbolType == SymbolType::Bracket)
//			{
//				/* Generate an error if there already exists a token or a bracket on the line */
//				if (line.Type == LineType::Token || line.Type == LineType::SectionStartBracket || line.Type == LineType::SectionEndBracket)
//					return false;
//				
//				/* Start section bracket */
//				if (symbol == NEW_SECTION_SYMBOL)
//				{
//					++indentationCounter;
//					line.Type = LineType::SectionStartBracket;
//				}
//				/* End section bracket  */
//				else if (symbol == END_SECTION_SYMBOL)
//				{
//					--indentationCounter;
//					line.Type = LineType::SectionEndBracket;
//				}
//			}
//			/* Symbol is a token delimiter symbol */
//			else if (symbolType == SymbolType::TokenDelimiter)
//			{
//				/* Generate an error if there already exists a bracket on the line */
//				if (line.Type == LineType::SectionStartBracket || line.Type == LineType::SectionEndBracket)
//					return false;
//
//				/* Start of a token */
//				if (!insideToken)
//				{
//					insideToken = true;
//					symbolsCountInToken = 0;
//				}
//				/* End of a token */
//				else if (insideToken)
//				{
//					insideToken = false;
//					/* Generate an error if the number of symbols inside the token is zero */
//					if (symbolsCountInToken == 0)
//						return false;
//				}
//				line.Type = LineType::Token;
//				++line.TokenSymbolCounter;
//			}
//			/* Symbol is an alphanumeric symbol */
//			else if (symbolType == SymbolType::Alphanumeric)
//			{
//				/* Generate an error if the symbol isn't between the token delimiter symbols */
//				if (!insideToken)
//					return false;
//				++symbolsCountInToken;
//			}
//		}
//
//		/* Generate an error if there are an uneven number of token delimiter symbols */
//		if (line.TokenSymbolCounter % 2 == 1)
//			return false;
//
//		/* If the line is a bracket line, then it needs to have a token in the earlier name (The name of the section)*/
//		if (line.Type == LineType::SectionStartBracket)
//		{
//			/* Out of index */
//			if (lineIndex == 0)
//				return false;
//			/* If line before it needs to be a token line */
//			else if (lines[lineIndex - 1].Type != LineType::Token)
//				return false;
//			/* The number of tokens in the line before it needs to be one */
//			else if (lines[lineIndex - 1].TokenSymbolCounter != 2)
//				return false;
//		}
//
//		/* Tokens that isn't a name and isn't between two brackets can't exist */
//		if (line.Type == LineType::Token && indentationCounter == 0)
//		{
//			/* Out of index */
//			if (lineIndex == _trimmedLines.size() - 1)
//				return false;
//			else if (GetLineType(_trimmedLines[lineIndex + 1]) != LineType::SectionStartBracket)
//				return false;
//		}
//
//		lines.push_back(line);
//	}
//
//	if (indentationCounter != 0)
//		return false;
//
//	return true;
//}

bool Parser::ValidateTokenStructure(const std::vector<std::string>& _trimmedLines)
{
	return true;
}

void Parser::ConvertLinesToTokens(std::vector<std::vector<std::string>>& _tokenizedLines, const std::vector<std::string>& _trimmedLines)
{
	for (unsigned int lineIndex = 0; lineIndex < _trimmedLines.size(); ++lineIndex)
	{
		_tokenizedLines.push_back(std::vector<std::string>());
		bool insideToken = false;
		std::string token = "";
		
		for (unsigned int symbolIndex = 0; symbolIndex < _trimmedLines[lineIndex].length(); ++symbolIndex)
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

void Parser::ConvertTokensToSections(Section* _sectionTree, const std::vector<std::vector<std::string>>& _tokenizedLines)
{
	Section* currentSection = _sectionTree;
	/* Loop through each line and add sections */
	for (unsigned int lineIndex = 0; lineIndex < _tokenizedLines.size(); ++lineIndex)
	{
		std::vector<std::string> tokenLine = _tokenizedLines[lineIndex];
		LineType lineType = GetLineType(tokenLine);
		switch (lineType)
		{
		case LineType::SectionStartBracket:
			AddNewSection(currentSection);
			currentSection->Name = _tokenizedLines[lineIndex - 1][0];
			break;
		case LineType::SectionEndBracket:
			EndSection(currentSection);
			break;
		case LineType::Token:
			AddTokens(currentSection, _tokenizedLines[lineIndex]);
			break;
		default:
			break;
		}
	}

	/* Make _sectionTree be the root again. */
	while (_sectionTree->ParentSection != 0)
		_sectionTree = _sectionTree->ParentSection;
}

Parser::LineType Parser::GetLineType(const std::vector<std::string>& _tokens)
{
	if (_tokens[0][0] == NEW_SECTION_SYMBOL)
		return LineType::SectionStartBracket;
	else if (_tokens[0][0] == END_SECTION_SYMBOL)
		return LineType::SectionEndBracket;
	else
		return LineType::Token;
}

void Parser::AddNewSection(Section* _currentSection)
{
	Section* lastSection = _currentSection;
	_currentSection = new Section();
	lastSection->SubSections->push_back(*_currentSection);
	_currentSection->ParentSection = lastSection;
}

void Parser::EndSection(Section* _currentSection)
{
	_currentSection = _currentSection->ParentSection;
}

void Parser::AddTokens(Section* _currentSection, const std::vector<std::string>& _tokens)
{
	/* Add a new token line */
	_currentSection->Tokens->push_back(new std::vector<std::string>());
	/* Add the tokens to the line */
	for (auto token : _tokens)
	{
		_currentSection->Tokens->back()->push_back(token);
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

