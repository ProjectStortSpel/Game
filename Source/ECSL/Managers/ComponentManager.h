#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <SDL/SDL.h>

#include <string>
#include <unordered_map>

namespace ECSL
{
	typedef size_t TypeId;

	class DECLSPEC ComponentManager
	{
	private:
		static int m_nextTableId;

	public:
		template<typename ComponentType>
		static TypeId GetTableId()
		{
			static TypeId m_id = ++m_nextTableId;
			return m_id;
		}

		static TypeId GetTableId(std::string _componentType)
		{
			static std::unordered_map<std::string, TypeId> m_stringTableId = std::unordered_map<std::string, TypeId>();
			// Try to find the id of the component type in the map. 
			// If it doesn't exist then create it, else return the found value 
			auto it = m_stringTableId.find(_componentType);
			if (it == m_stringTableId.end())
			{
				TypeId newId = ++ComponentManager::m_nextTableId;
				m_stringTableId.insert(std::pair<std::string, TypeId>(_componentType, newId));
				return newId;
			}
			else
				return it->second;
		}
	};

	int ComponentManager::m_nextTableId = -1;
}

#endif