#ifndef COMPONENTFILTER_H
#define COMPONENTFILTER_H

#include <SDL/SDL.h>
#include <vector>
#include "ECSL/Framework/Common/BitSet.h"
#include <string>

namespace ECSL
{
	enum DECLSPEC FilterType
	{
		Mandatory,
		RequiresOneOf,
		Excluded
	};

	struct DECLSPEC ComponentFilter
	{
	public:
		ComponentFilter();
		~ComponentFilter();

		void Add(const std::string& _componentType);

		inline const BitSet::DataType* const	GetBitSet()					{ return m_bitSet; }
		inline std::vector<std::string>*		GetComponentTypes() const	{ return m_addedComponentTypes; }

		void SetBitSet(BitSet::DataType* _bitSet);

	private:
		BitSet::DataType* m_bitSet;
		std::vector<std::string>* m_addedComponentTypes;
	};
}


#endif