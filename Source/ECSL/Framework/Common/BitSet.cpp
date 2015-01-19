#include "BitSet.h"
using namespace ECSL;

BitSet::DataType* BitSet::BitSetConverter::ValueToBitSet(unsigned int _numberToConvert, unsigned int _maxNumberOfBits)
{
	/*	Calculate how many DataType(s) needed to cover all numbers	*/
	int	numberOfInts = GetDataTypeCount(_maxNumberOfBits);
	DataType* newBitSet = GenerateBitSet(_maxNumberOfBits);

	return newBitSet;
}

BitSet::DataType* BitSet::BitSetConverter::ArrayToBitSet(const std::vector<unsigned int>& _numbersToConvert, unsigned int _maxNumberOfBits)
{
	/*	Calculate how many DataType(s) needed to cover all numbers	*/
	int	numberOfInts = GetDataTypeCount(_maxNumberOfBits);
	DataType* newBitSet = GenerateBitSet(_maxNumberOfBits);

	/*	Go through all numbers and place them in the correct bitset	*/
	for (unsigned int i = 0; i < _numbersToConvert.size(); ++i)
	{
		unsigned int currentBit = _numbersToConvert.at(i);

		/*	Calculate at which index the current number should be represented	*/
		unsigned int bitSetIndex = GetBitSetIndex(currentBit);
		unsigned int bitIndex = GetBitIndex(currentBit);

		/*	Set the correct 'bit position'	*/
		newBitSet[bitSetIndex] |= ((DataType)1) << bitIndex;
	}
	return newBitSet;
}

void BitSet::BitSetConverter::BitSetToArray(std::vector<unsigned int>& _out, const BitSet::DataType* const _bitmask, unsigned int _bitmaskCount)
{
	unsigned int bitsPerInt = GetDataTypeByteSize() * 8;
	unsigned int index = 0;

	/*	Go through all bitmasks	*/
	for (unsigned int bitSetIndex = 0; bitSetIndex < _bitmaskCount; ++bitSetIndex)
	{
		/*	Iterate over all bits inside each bitmask	*/
		for (unsigned int bitIndex = 0; bitIndex < bitsPerInt; ++bitIndex)
		{
			if (_bitmask[bitSetIndex] & ((ECSL::BitSet::DataType)1 << bitIndex))
				_out.push_back(index);
			++index;
		}
	}
}