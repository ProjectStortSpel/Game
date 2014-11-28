#ifndef PARSER_H
#define PARSER_H

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <map>

#include "Section.h"
#include "../Components/ComponentType.h"

namespace ECSL
{
	class Parser
	{
	public:
		Parser();
		DECLSPEC ~Parser();

		bool ParseFile(Section& _out, const std::string& _filePath);
	private:
		enum LineType { None, SectionStartBracket, SectionEndBracket, Token };
		enum SymbolType { Alphanumeric, Bracket, TokenDelimiter, EmptySpace, Invalid };
		struct Line
		{
			std::string Text;
			LineType Type;
			unsigned int DelimiterSymbolCounter;

			Line() : Text(""), Type(None), DelimiterSymbolCounter(0) { }
		};

		const char DELIMITER_SYMBOL = '"';
		const char NEW_SECTION_SYMBOL = '{';
		const char END_SECTION_SYMBOL = '}';

		bool ValidateSymbols(const std::string& _fileLine);
		void TrimLine(std::string& _fileLine);
		void GetLineData(Line& _line, const std::string& _fileLine);

		bool ValidateSyntax(const std::vector<Line>& _lines);
		bool ValidateLineSyntax(const std::vector<Line>& _lines);
		bool ValidateLineDependencies(const std::vector<Line>& _lines);
		void ConvertLinesToTokens(std::vector<std::vector<std::string>>& _tokenizedLines, const std::vector<Line>& _lines);
		
		void ConvertTokensToSections(Section* _sectionTree, const std::vector<std::vector<std::string>>& _tokenizedLines, const std::vector<Line>& _lines);
		inline void AddTokens(Section* _currentSection, const std::vector<std::string>& _tokens);

		inline SymbolType GetSymbolType(char _symbol);
		inline bool IsLineEmpty(const std::string& _line);
	};
}

#endif