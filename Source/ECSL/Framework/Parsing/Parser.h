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
	class DECLSPEC Parser
	{
	public:
		Parser();
		~Parser();

		Section* ParseFile(const std::string& _filePath);

	private:
		enum LineType { None, SectionStartBracket, SectionEndBracket, Token };
		enum SymbolType { Alphanumeric, Bracket, TokenDelimiter, EmptySpace, Invalid };
		struct Line
		{
			LineType Type;
			int TokenSymbolCounter;

			Line() : Type(None), TokenSymbolCounter(0) { }
		};

		const char DELIMITER_SYMBOL = '"';
		const char NEW_SECTION_SYMBOL = '{';
		const char END_SECTION_SYMBOL = '}';

		std::map<std::string, int>* m_byteConversion;

		bool ValidateSymbols(const std::string& _line);
		void TrimLine(std::string& _line);

		bool ValidateTokenStructure(const std::vector<std::string>& _trimmedLine);
		bool ValidateTokenLine();
		void ConvertLinesToTokens(std::vector<std::vector<std::string>>& _tokenizedLines, const std::vector<std::string>& _trimmedLines);
		
		void ConvertTokensToSections(Section* _sectionTree, const std::vector<std::vector<std::string>>& _tokenizedLines);
		inline LineType GetLineType(const std::vector<std::string>& _tokens);
		inline void AddNewSection(Section* _currentSection);
		inline void EndSection(Section* _currentSection);
		inline void AddTokens(Section* _currentSection, const std::vector<std::string>& _tokens);

		inline SymbolType GetSymbolType(char _symbol);
		inline bool IsLineEmpty(const std::string& _line);
	};
}

#endif