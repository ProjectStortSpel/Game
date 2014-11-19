#include "Parser.h"
#include <fstream>
#include <string>
#include <stdio.h>

using namespace ECSL;

Parser::Parser()
{

}
Parser::~Parser()
{

}

void Parser::ParseFile(char* _filename)
{
	std::ifstream fStream;
	fStream.open(_filename);

	if (!fStream)
		return;

	std::string tText;
	while (!fStream.eof())
	{
		fStream >> tText;


		printf(tText.c_str());
	}

}