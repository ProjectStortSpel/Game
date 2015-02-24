#include "LuaCombinationMath.h"

namespace LuaBridge
{
	namespace CombinationMath
	{
		//std::vector<std::vector<unsigned char>> m_combinations;
		//std::vector<std::vector<unsigned char>> m_permutations;
		//char* m_combinations;
		//char* m_permutations;

		void Embed(lua_State* L)
		{
			LuaEmbedder::AddFunction(L, "Combinations", &LuaCombinations, "CombinationMath");
			LuaEmbedder::AddFunction(L, "Permutations", &LuaPermutations, "CombinationMath");
		}

		int LuaCombinations(lua_State* L)
		{
			unsigned int noToChooseFrom = LuaEmbedder::PullInt(L, 1);
			unsigned int noToPick = LuaEmbedder::PullInt(L, 2);
			unsigned int* size = new unsigned int;
			*size = noToPick;
			char *pickedCards = new char[noToPick];

			unsigned int noOfCombinations = NoOfCombinations(noToChooseFrom, noToPick);

			char *combinations = new char[noOfCombinations * noToPick + 1];
			unsigned int *combinationsSize = new unsigned int;
			*combinationsSize = 0;
			
			Combinations(noToChooseFrom, noToPick, pickedCards, size, combinations, combinationsSize);
			
			combinations[noOfCombinations * noToPick] = '\0';

			LuaEmbedder::PushString(L, std::string((const char*)combinations));

			delete size;
			delete [] combinations;
			delete combinationsSize;
			delete [] pickedCards;

			return 1;
		}

		int LuaPermutations(lua_State* L)
		{
			unsigned int noToChooseFrom = LuaEmbedder::PullInt(L, 1);
			unsigned int noToPick = LuaEmbedder::PullInt(L, 2);
			unsigned int* size = new unsigned int;
			*size = noToPick;
			char *pickedCards = new char[noToPick];

			unsigned int noOfCombinations = NoOfCombinations(noToChooseFrom, noToPick);
			unsigned int noOfPermutations = NoOfPermutations(noToChooseFrom, noToPick);

			printf("permutations %i\n", noOfPermutations);

			char *combinations = new char[noOfCombinations * noToPick];
			char *permutations = new char[noOfPermutations * noToPick + 1];

			unsigned int *combinationsSize = new unsigned int;
			unsigned int *permutationsSize = new unsigned int;
			*combinationsSize = 0;
			*permutationsSize = 0;

			Combinations(noToChooseFrom, noToPick, pickedCards, size, combinations, combinationsSize);

			Permutations(combinations, combinationsSize, permutations, permutationsSize, noToPick);

			permutations[noOfPermutations * noToPick] = '\0';

			LuaEmbedder::PushString(L, std::string((const char*)permutations));

			delete size;
			delete combinationsSize;
			delete permutationsSize;
			delete[] combinations;
			delete[] permutations;
			delete[] pickedCards;

			return 1;
		}


		void Combinations(int _noToChooseFrom, int _noToPick, char* _pickedCards, unsigned int* _pickedCardsSize, char* _combinations, unsigned int* _combinationsSize)
		{
			for (int i = _noToChooseFrom; i >= _noToPick; i--)
			{
				/* Choose the first element.*/
				_pickedCards[_noToPick - 1] = i;

				/* If we still need to choose more elements, recursively split into smaller problems. */
				if (_noToPick > 1)
				{
					Combinations(i - 1, _noToPick - 1, _pickedCards, _pickedCardsSize, _combinations, _combinationsSize);
				}
				else
				{
					/* Save a solution.*/
					for (int j = 0; j < *_pickedCardsSize; j++)
					{
						_combinations[*_combinationsSize] = _pickedCards[j];
						*_combinationsSize = *_combinationsSize + 1;
					}

				}
			}
		}

		void Permutations(char* _combinations, unsigned int* _combinationsSize, char* _permutations, unsigned int* _permutationsSize, unsigned int _cardsToPick)
		{
			int loops = 0;
			
			clock_t start, end;
			
			start = clock();
			
			for (unsigned int i = 0; i < *_combinationsSize; i += _cardsToPick)
			{
				do
				{
					for (unsigned int j = 0; j < _cardsToPick; j++)
					{
						_permutations[*_permutationsSize] = _combinations[i + j];
						*_permutationsSize = *_permutationsSize + 1;
					}
			
					loops++;
				} while (std::next_permutation(_combinations + i, _combinations + i + _cardsToPick));
			}
			
			end = clock();
			
			// TODO: REMOVE INCLUDE TIME.
			std::cout << "Found " << loops << " permutations in " << (end - start) * 1000 / CLOCKS_PER_SEC << " milliseconds." << std::endl;
		}

		unsigned int NoOfCombinations(unsigned int _noToChooseFrom, unsigned int _noToPick)
		{
			return Factorial(_noToChooseFrom) / (Factorial(_noToChooseFrom - _noToPick) * Factorial(_noToPick));
		}

		unsigned int NoOfPermutations(unsigned int _noToChooseFrom, unsigned int _noToPick)
		{
			return Factorial(_noToChooseFrom) / Factorial(_noToChooseFrom - _noToPick);
		}

		unsigned int Factorial(unsigned int _n)
		{
			if (_n == 1 || _n == 0)
			{
				return 1;
			}
			else
			{
				return Factorial(_n - 1) * _n;
			}
		}
	}
}