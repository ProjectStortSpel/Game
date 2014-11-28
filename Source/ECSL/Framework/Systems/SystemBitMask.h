#ifndef SYSTEMBITMASK_H
#define SYSTEMBITMASK_H

#include <SDL/SDL.h>
#include <vector>
#include "ECSL/Framework/Common/BitSet.h"
#include <string>

namespace ECSL
{
	enum DECLSPEC ComponentFilter
	{
		Mandatory,
		RequiresOneOf,
		Excluded
	};

	struct DECLSPEC SystemBitmask
	{
	public:
		SystemBitmask()
		{
			m_addedComponentTypes = new std::vector<std::string>();
			m_bitSet = 0;
		}
		~SystemBitmask()
		{
			if (m_addedComponentTypes)
				delete(m_addedComponentTypes);

			if (m_bitSet)
				delete m_bitSet;  
		}

		std::vector<std::string>* GetComponentTypes() const { return m_addedComponentTypes; }

		void Add(std::string _componentType)
		{
			/*	Check if the component type is already added	*/
			for (unsigned int n = 0; n < m_addedComponentTypes->size(); ++n)
				if (_componentType == m_addedComponentTypes->at(n))
					return;

			/*	Add the component type	*/
			m_addedComponentTypes->push_back(_componentType);
		}

		void SetBitmask(BitSet::DataType* _bitSet)
		{
			m_bitSet = _bitSet;
		}

	private:
		BitSet::DataType* m_bitSet;
		std::vector<std::string>* m_addedComponentTypes;

	};
}


#endif