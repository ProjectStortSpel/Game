#ifndef COMPONENTTYPEMANAGER_H
#define COMPONENTTYPEMANAGER_H

#include <SDL/SDL.h>
#include <string>
#include <unordered_map>
#include <map>

#include "../Framework/Components/ComponentType.h"
#include "../Framework/Parsing/ComponentTypeReader.h"

namespace ECSL
{
	typedef size_t TypeId;

	namespace ComponentManagerData
	{
		static int NextTableId = -1;
	}

	class DECLSPEC ComponentTypeManager
	{
	public:
		static ComponentTypeManager& GetInstance();
		~ComponentTypeManager();

		template<typename ComponentType>
		void AddComponentType();
		void AddComponentType(ComponentType& _componentType);
		void LoadComponentTypesFromDirectory(const std::string& _directoryPath);
		void LoadComponentTypesFromFile(const std::string& _filePath);

		ComponentType* GetComponentType(int _componentTypeId);

		template<typename ComponentType>
		static TypeId GetTableId();
		static TypeId GetTableId(const std::string& _componentType);
	private:
		Parser m_parser;
		ComponentTypeReader m_componentTypeReader;
		std::map<int, ComponentType*>* m_loadedComponentTypes;

		ComponentTypeManager();
	};

	template<typename ComponentType>
	void ComponentTypeManager::AddComponentType()
	{
		TypeId componentId = ComponentManager::GetTableId<ComponentType>();

		auto it = m_loadedComponentTypes->find(componentId);
		if (it == m_loadedComponentTypes->end())
			m_loadedComponentTypes->operator[](componentId) = new ComponentType();
	}

	template<typename ComponentType>
	TypeId ComponentTypeManager::GetTableId()
	{
		static TypeId m_id = ++ComponentManagerData::NextTableId;
		return m_id;
	}
}

#endif