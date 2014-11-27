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

	class DECLSPEC ComponentTypeManager
	{
	public:
		~ComponentTypeManager();
		static ComponentTypeManager& GetInstance();

		template<typename ComponentType>
		void AddComponentType();
		void AddComponentType(ComponentType& _componentType);
		void LoadComponentTypesFromDirectory(const std::string& _directoryPath);
		void LoadComponentTypesFromFile(const std::string& _filePath);

		ComponentType* GetComponentType(int _componentTypeId);
		unsigned int GetComponentTypeCount(){ return m_componentTypes->size(); }

		template<typename ComponentType>
		TypeId GetTableId();
		TypeId GetTableId(const std::string& _componentType);
	private:
		int m_nextTableId = -1;
		Parser* m_parser;
		ComponentTypeReader* m_componentTypeReader;
		std::map<int, ComponentType*>* m_componentTypes;
		std::unordered_map<std::string, TypeId>* m_stringTableId;

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
		static TypeId m_id = ++m_nextTableId;
		return m_id;
	}
}

#endif