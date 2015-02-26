#ifndef COMBINATIONMATH_H
#define COMBINATIONMATH_H


#include <LuaEmbedder/LuaEmbedder.h>
#include <algorithm>
#include <vector>

namespace LuaBridge
{
	namespace CombinationMath
	{
		void Embed(lua_State* L);
		int LuaCombinations(lua_State* L);
		int LuaPermutations(lua_State* L);

		void Combinations(int _noToChooseFrom, int _noToPick, char* _pickedCards, unsigned int* _pickedCardSize, char* _combinations, unsigned int* _combinationsSize);
		void Permutations(char* _combinations, unsigned int* _combinationsSize, char* _permutations, unsigned int* _permutationsSize, unsigned int _cardsToPick);

		unsigned int NoOfCombinations(unsigned int _noToChooseFrom, unsigned int _noToPick);
		unsigned int NoOfPermutations (unsigned int _noToChooseFrom, unsigned int _noToPick);
		unsigned int Factorial(unsigned int _n);
	}
}

#endif