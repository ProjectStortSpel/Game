#ifndef BITSET_H
#define BITSET_H

#include <SDL/SDL.h>
#include <math.h>
#include <vector>
#include <inttypes.h>
#include <limits>

namespace ECSL
{
	namespace BitSet
	{
#if defined(__ANDROID__) || defined(__IOS__)
		typedef unsigned int DataType;
		const unsigned int BitSetBitShift = 5;
#else
		typedef uint64_t DataType;
		const unsigned int BitSetBitShift = 6;
#endif

		const DataType BitSetIndexMask = (std::numeric_limits<DataType>::max)() ^ ((sizeof(DataType)* 8) - 1);
		const DataType BitIndexMask = (sizeof(DataType)* 8) - 1;
		const unsigned int ByteCountMask = (std::numeric_limits<unsigned int>::max)() ^ 7;

		bool BitSetMatchesMasks(unsigned int _bitSetCount, const BitSet::DataType* _bitSet, const BitSet::DataType* _mandatoryMask, const BitSet::DataType* _oneOfMask, const BitSet::DataType* _exclusionMask);

		inline DECLSPEC unsigned int GetDataTypeByteSize()
		{
			return sizeof(DataType);
		}
		inline DECLSPEC DataType* GenerateBitSet(unsigned int _bitCount)
		{ 
			return (DataType*)calloc((_bitCount / GetDataTypeByteSize()) + 1, GetDataTypeByteSize());
		}
		inline DECLSPEC unsigned int GetBitSetIndex(unsigned int _bitIndex)
		{
			return (_bitIndex & BitSetIndexMask) >> BitSetBitShift;
		}
		inline DECLSPEC unsigned int GetBitIndex(unsigned int _bitIndex)
		{
			return (_bitIndex & BitIndexMask);
		}
		inline DECLSPEC unsigned int GetByteCount(unsigned int _bitCount)
		{
			return (((_bitCount - 1) & ByteCountMask) >> 3) + 1;
		}
		inline DECLSPEC unsigned int GetDataTypeCount(unsigned int _bitCount)
		{
			return GetBitSetIndex(_bitCount) + 1;
		}

		namespace BitSetConverter
		{
			DECLSPEC DataType* ValueToBitSet(unsigned int _numberToConvert, unsigned int _maxNumberOfBits);
			DECLSPEC DataType* ArrayToBitSet(const std::vector<unsigned int>& _numbersToConvert, unsigned int _maxNumberOfBits);
			DECLSPEC void BitSetToArray(std::vector<unsigned int>& _out, const DataType* const _bitmask, unsigned int _bitmaskCount);
		}
	}
}

#endif