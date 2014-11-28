#include "BitSet.h"
using namespace ECSL;

BitSet::BitSetConverter& BitSet::BitSetConverter::GetInstance()
{
	static BitSet::BitSetConverter* instance = new BitSet::BitSetConverter();
	return *instance;
}

BitSet::BitSetConverter::BitSetConverter()
{
	/*	Calculate power of two	*/
	for (int n = 0; n < sizeof(BitSet::DataType) * 8; ++n)
		m_powerOfTwo[n] = (BitSet::DataType)pow(2.0f, n);
}

BitSet::BitSetConverter::~BitSetConverter()
{
}

void BitSet::BitSetConverter::ValueToBitSet(DataType* _out, unsigned int _numberToConvert, unsigned int _maxNumberOfBits)
{
	/*	Calculate how many DataType(s) needed to cover all numbers	*/
	int	numberOfInts = GetIntCount(_maxNumberOfBits);
	DataType* newBitSet = GenerateBitSet(_maxNumberOfBits);


}

void BitSet::BitSetConverter::ArrayToBitSet(BitSet::DataType* _out, const std::vector<unsigned int>& _numbersToConvert, unsigned int _maxNumberOfBits)
{
	/*	Calculate how many DataType(s) needed to cover all numbers	*/
	int	numberOfInts = GetIntCount(_maxNumberOfBits);
	DataType* newBitSet = GenerateBitSet(_maxNumberOfBits);

	/*	Go through all numbers and place them in the correct bitset	*/
	for (unsigned int i = 0; i < _numbersToConvert.size(); ++i)
	{
		int currentInt = _numbersToConvert.at(i);

		/*	Calculate at which index the current number should be represented	*/
		int	bitmaskIndex = (int)floor((float)currentInt / (GetIntByteSize() * 8));
		int	bitIndex = currentInt % (GetIntByteSize() * 8);

		/*	Set the correct 'bit position'	*/
		newBitSet[bitmaskIndex] += m_powerOfTwo[bitIndex] | 0;
	}

	_out = newBitSet;
}

void BitSet::BitSetConverter::BitSetToArray(std::vector<unsigned int>& _out, BitSet::DataType* _bitmask, unsigned int _bitmaskCount)
{
	/*	Allocate a bool array with the size of the number of bits required to cover	*/
	int bitCount = _bitmaskCount * GetIntByteSize() * 8;

	/*	Go through all bitmasks	*/
	for (unsigned int bitmaskIndex = 0; bitmaskIndex < _bitmaskCount; ++bitmaskIndex)
	{
		/*	Iterate over all bits inside each bitmask	*/
		for (unsigned int bitIndex = 0; bitIndex < GetIntByteSize() * 8; ++bitIndex)
		{
			/*	Add the number to the resulting vector if it is in the bitmask	*/
			if ((m_powerOfTwo[bitIndex] & _bitmask[bitmaskIndex]) == m_powerOfTwo[bitIndex])
				_out.push_back(bitmaskIndex * GetIntByteSize() * 8 + bitIndex);
		}
	}
}