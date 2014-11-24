#include "Section.h"

using namespace ECSL;

Section::Section() : Name("")
{
	Tokens = new std::vector<std::vector<std::string>*>();
	SubSections = new std::vector<Section>();
}

Section::~Section()
{
	for (int lineIndex = Tokens->size() - 1; lineIndex >= 0; --lineIndex)
	{
		std::vector<std::string>* tokenLine = Tokens->at(lineIndex);
		tokenLine->erase(tokenLine->begin(), tokenLine->end());
		Tokens->pop_back();
		if (tokenLine != 0)
			delete(tokenLine);
	}
	if (Tokens != 0 && Tokens->size() != 0)
		delete(Tokens);
	SubSections->erase(SubSections->begin(), SubSections->end());
	if (SubSections != 0 && SubSections->size() != 0)
		delete(SubSections);
}