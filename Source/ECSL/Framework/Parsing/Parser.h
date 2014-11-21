#ifndef PARSER_H
#define PARSER_H

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include <map>

#include "../Components/ComponentType.h"

namespace ECSL
{
	/* Section = The part between { and } */
	struct Section
	{
		std::string Name;
		std::vector<std::vector<std::string>> Tokens;
		std::vector<Section> SubSections;
	};

	class DECLSPEC Parser
	{
	public:
		Parser();
		~Parser();

		std::vector<Section>* ParseFile(const std::string& _filePath);

	private:
		struct Line
		{
			enum LineType { None, SectionStartBracket, SectionEndBracket, Token };

			LineType Type;
			int TokenSymbolCounter;

			Line() : Type(None), TokenSymbolCounter(0) { }
		};

		enum SymbolType { Alphanumeric, Bracket, TokenDelimiter, EmptySpace, Invalid };

		const char DELIMITER_SYMBOL = '"';
		const char NEW_SECTION_SYMBOL = '{';
		const char END_SECTION_SYMBOL = '}';

		std::map<std::string, int>* m_byteConversion;

		bool TokensToSections(std::vector<Section>& _sections, const std::vector<std::vector<std::string>>& _tokenizedLines);

		bool ValidateSymbols(const std::string& _line);
		bool ValidateTokenStructure(const std::vector<std::string>& _trimmedLine);
		inline bool IsLineEmpty(const std::string& _line);
		void TrimLine(std::string& _line);
		void ConvertLinesToTokens(std::vector<std::vector<std::string>>& _tokenizedLines, const std::vector<std::string>& _trimmedLines);
		inline SymbolType GetSymbolType(char _symbol);
	};
}

#endif