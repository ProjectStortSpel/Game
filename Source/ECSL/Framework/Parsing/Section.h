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
		std::vector<std::vector<std::string>> Tokens;
		std::vector<Section> SubSections;
	};
}

#endif