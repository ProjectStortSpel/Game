#include "BitSet.h"

using namespace ECSL;

bool BitSet::BitSetMatchesMasks(unsigned int _bitSetCount, const BitSet::DataType* _bitSet, const BitSet::DataType* _mandatoryMask, const BitSet::DataType* _oneOfMask, const BitSet::DataType* _exclusionMask)
{
	char requiresOneOf = 0;

	/* Checks every component filter (breaks if fails) */
	for (unsigned int i = 0; i < _bitSetCount; ++i)
	{
		/* Entity doesn't have atleast one of the must-have components */
		if (!((_mandatoryMask[i] & _bitSet[i]) == _mandatoryMask[i]))
			return false;

		/* Entity has atleast one of the excluded components  */
		else if (_exclusionMask[i] && (_exclusionMask[i] & _bitSet[i]) != 0)
			return false;

		/* Entity has none of the atleast-one-of components */
		else if (_oneOfMask[i] && requiresOneOf < 2)
		{
			requiresOneOf = 1;
			if ((_oneOfMask[i] & _bitSet[i]) != 0)
				requiresOneOf = 2;
		}
	}

	if (requiresOneOf == 1)
		return false;

	return true;
}

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