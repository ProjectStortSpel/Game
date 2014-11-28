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
	class DECLSPEC ComponentTypeManager
	{
	public:
		~ComponentTypeManager();
		static ComponentTypeManager& GetInstance();

		void LoadComponentTypesFromDirectory(const std::string& _directoryPath);
		void LoadComponentTypesFromFile(const std::string& _filePath);

		ComponentType* GetComponentType(int _componentTypeId);
		unsigned int GetComponentTypeCount(){ return m_componentTypes->size(); }

		template<typename ComponentType>
		unsigned int GetTableId();
		unsigned int GetTableId(const std::string& _componentType);
	private:
		ComponentTypeManager();

		int m_nextTableId;
		Parser* m_parser;
		ComponentTypeReader* m_componentTypeReader;
		std::map<int, ComponentType*>* m_componentTypes;
		std::unordered_map<std::string, unsigned int>* m_stringTableId;

		template<typename ComponentType>
		void AddComponentType();
		void AddComponentType(ComponentType& _componentType);
	};

	template<typename ComponentType>
	void ComponentTypeManager::AddComponentType()
	{
		unsigned int componentId = ComponentManager::GetTableId<ComponentType>();

		auto it = m_loadedComponentTypes->find(componentId);
		if (it == m_loadedComponentTypes->end())
			m_loadedComponentTypes->operator[](componentId) = new ComponentType();
	}

	template<typename ComponentType>
	unsigned int ComponentTypeManager::GetTableId()
	{
		static unsigned int m_id = ++m_nextTableId;
		return m_id;
	}
}

#endif