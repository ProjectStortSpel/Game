#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <SDL/SDL.h>
#include "ECSL/Framework/Components/ComponentType.h"
#include <string>
#include <unordered_map>
#include <map>

namespace ECSL
{
	typedef size_t TypeId;

	namespace ComponentManagerData
	{
		static int NextTableId = -1;
	}

#pragma region Component Type Manager
	class DECLSPEC ComponentTypeManager
	{
	public:
		static ComponentTypeManager& GetInstance();
		~ComponentTypeManager();

		template<typename ComponentType>
		void AddComponentType();

		void AddComponentType(std::string _componentType);

		ComponentTypeInterface* GetComponentType(int _componentTypeId);

	private:
		ComponentTypeManager();
		std::map<int, ComponentTypeInterface*>* m_loadedComponentTypes;

	};

	template<typename ComponentType>
	void ComponentTypeManager::AddComponentType()
	{
		//	Get the correct
		TypeId componentId = ComponentManager::GetTableId<ComponentType>();

		auto it = m_loadedComponentTypes->find(componentId);
		if (it == m_loadedComponentTypes->end())
			m_loadedComponentTypes->operator[](componentId) = new ComponentType();
	}
#pragma endregion

#pragma region Component Manager
	class DECLSPEC ComponentManager
	{
	public:

		template<typename ComponentType>
		static TypeId GetTableId()
		{
			static TypeId m_id = ++ComponentManagerData::NextTableId;
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
				TypeId newId = ++ComponentManagerData::NextTableId;
				m_stringTableId.insert(std::pair<std::string, TypeId>(_componentType, newId));
				return newId;
			}
			else
				return it->second;
		}
	};

#pragma endregion
}

#endif