#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H


#include <LuaEmbedder/LuaEmbedder.h>
//#include <glm/glm.hpp>
//#include <vector>

namespace LuaBridge
{
	namespace Permutations
	{
		int LuaPermutations(lua_State* L);

		void Combinations(int _noToChooseFrom, int _noToPick, unsigned char* _array, int _size);
		void Permutations();
	}
}

#endif