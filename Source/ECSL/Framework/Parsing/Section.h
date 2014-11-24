#ifndef SECTION_H
#define SECTION_H

#include <SDL/SDL.h>

#include <string>
#include <unordered_map>

namespace ECSL
{
	struct Section
	{
		std::string Name;
		std::vector<std::vector<std::string>*>* Tokens;
		Section* ParentSection;
		std::vector<Section>* SubSections;

		Section();
		~Section();
	};
}

#endif