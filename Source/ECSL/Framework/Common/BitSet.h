#ifndef BITSET_H
#define BITSET_H

#include <SDL/SDL.h>
#include <math.h>
#include <vector>
#include <inttypes.h>

namespace ECSL
{
	namespace BitSet
	{
		typedef uint64_t DataType;

		inline DECLSPEC unsigned int GetIntByteSize()
		{
			return sizeof(DataType);
		}
		inline DECLSPEC DataType* GenerateBitSet(unsigned int _bitCount)
		{ 
			return (DataType*)calloc((_bitCount / GetIntByteSize()) + 1, GetIntByteSize());
		}
		inline DECLSPEC unsigned int GetByteCount(unsigned int _bitCount) 
		{ 
			return (unsigned int)ceilf((float)_bitCount / 8);
		}
		inline DECLSPEC unsigned int GetIntCount(unsigned int _bitCount) 
		{ 
			return (unsigned int)ceilf((float)_bitCount / (GetIntByteSize() * 8));
		}
		inline DECLSPEC unsigned int GetBitSetIndex(unsigned int _bitIndex)
		{
			return (unsigned int)floor((float)_bitIndex / (BitSet::GetIntByteSize() * 8));
		}
		inline DECLSPEC unsigned int GetBitIndex(unsigned int _bitIndex)
		{
			return _bitIndex % (GetIntByteSize() * 8);
		}

		class DECLSPEC BitSetConverter
		{
		public:
			~BitSetConverter();
			static BitSetConverter& GetInstance();

			void ValueToBitSet(DataType* _out, unsigned int _numberToConvert, unsigned int _maxNumberOfBits);
			void ArrayToBitSet(DataType* _out, const std::vector<unsigned int>& _numbersToConvert, unsigned int _maxNumberOfBits);
			void BitSetToArray(std::vector<unsigned int>& _out, DataType* _bitmask, unsigned int _bitmaskCount);

		private:
			BitSetConverter();
			DataType m_powerOfTwo[sizeof(DataType) * 8];

		};

	}
}

#endif