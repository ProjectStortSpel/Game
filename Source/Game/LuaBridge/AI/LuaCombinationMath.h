#ifndef COMBINATIONMATH_H
#define COMBINATIONMATH_H


#include <LuaEmbedder/LuaEmbedder.h>
#include <algorithm>
#include <vector>
#include <time.h>

//#include <glm/glm.hpp>

namespace LuaBridge
{
	namespace CombinationMath
	{
		//extern std::vector<std::vector<unsigned char>> m_combinations;
		//extern std::vector<std::vector<unsigned char>> m_permutations;

		//extern char* m_combinations;
		//extern char* m_permutations;

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