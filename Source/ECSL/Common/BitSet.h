#ifndef BITSET_H
#define BITSET_H

#include <SDL/SDL.h>

namespace ECSL
{
	namespace BitSet
	{
		typedef unsigned __int64 DataType;

		inline DECLSPEC unsigned __int64* GenerateBitSet(unsigned int _bitCount) 
		{ 
			return new unsigned __int64[(_bitCount / GetIntByteSize()) + 1]; 
		}
		inline DECLSPEC unsigned int GetByteCount(unsigned int _bitCount) 
		{ 
			return (unsigned int)ceilf((float)_bitCount / 8);
		}
		inline DECLSPEC unsigned int GetIntCount(unsigned int _bitCount) 
		{ 
			return (unsigned int)ceilf((float)_bitCount / (GetIntByteSize() * 8));
		}
		inline DECLSPEC unsigned int GetIntByteSize() 
		{ 
			return sizeof(DataType);
		}
	}
}

#endif