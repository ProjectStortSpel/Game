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

BitSet::DataType* BitSet::BitSetConverter::GenerateBitmask(std::vector<unsigned int>* _numbersToConvert, unsigned int _maxNumberOfBits)
{
	/*	Calculate how many DataType(s) needed to cover all numbers	*/
	int	numberOfInts = GetIntCount(_maxNumberOfBits);
	DataType* newBitSet = GenerateBitSet(_maxNumberOfBits);

	/*	Set all bits to zero	*/
	for (int n = 0; n < numberOfInts; ++n)
		newBitSet[n] = 0;

	/*	Go through all numbers and place them in the correct bitset	*/
	for (int i = 0; i < _numbersToConvert->size(); ++i)
	{
		int currentInt = _numbersToConvert->at(i);

		/*	Calculate at which index the current number should be represented	*/
		int	bitmaskIndex = floor((float)currentInt / (GetIntByteSize() * 8));
		int	bitIndex = currentInt % (GetIntByteSize() * 8);

		/*	Set the correct 'bit position'	*/
		newBitSet[bitmaskIndex] += m_powerOfTwo[bitIndex] | 0;
	}

	return newBitSet;
}

std::vector<unsigned int>* BitSet::BitSetConverter::GenerateBoolArray(BitSet::DataType* _bitmask, unsigned int _bitmaskCount)
{
	/*	Allocate a bool array with the size of the number of bits required to cover	*/
	int bitCount = _bitmaskCount * GetIntByteSize() * 8;
	std::vector<unsigned int>* convertedNumbers = new std::vector<unsigned int>();

	/*	Go through all bitmasks	*/
	for (unsigned int bitmaskIndex = 0; bitmaskIndex < _bitmaskCount; ++bitmaskIndex)
	{

		/*	Iterate over all bits inside each bitmask	*/
		for (unsigned int bitIndex = 0; bitIndex < GetIntByteSize() * 8; ++bitIndex)
		{
			/*	Add the number to the resulting vector if it is in the bitmask	*/
			if ((m_powerOfTwo[bitIndex] & _bitmask[bitmaskIndex]) == m_powerOfTwo[bitIndex])
				convertedNumbers->push_back(bitmaskIndex * GetIntByteSize() * 8 + bitIndex);
		}
	}
		

	return convertedNumbers;
}