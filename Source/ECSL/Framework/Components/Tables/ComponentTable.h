#ifndef COMPONENTTABLE_H
#define COMPONENTTABLE_H

#include <SDL/SDL.h>
#include <string>
#include "DataTable.h"
#include "ECSL/Managers/ComponentTypeManager.h"

namespace ECSL
{
	class ComponentTable
	{
	public:
		ComponentTable(DataTable* _dataTable, unsigned int _componentTypeId);
		~ComponentTable();

		DataLocation GetComponent(unsigned int _entityId, const std::string& _variableName);
		DataLocation GetComponent(unsigned int _entityId, unsigned int _index);
		template<typename ReturnType>
		ReturnType* GetComponent(unsigned int _entityId, const std::string& _variableName);
		template<typename ReturnType>
		ReturnType* GetComponent(unsigned int _entityId, unsigned int _index);

		void SetComponent(unsigned int _entityId, const std::string& _variableName, void* _data);
		void SetComponent(unsigned int _entityId, unsigned int _index, void* _data, unsigned int _byteSize);

		void ClearComponent(unsigned int _entityId);

		unsigned int GetMemoryAllocated();

	private:
		unsigned int m_componentTypeId;
		ComponentType* m_componentType;
		DataTable* m_dataTable;
	};

	template<typename ReturnType>
	ReturnType* ComponentTable::GetComponent(unsigned int _entityId, const std::string& _variableName)
	{
		return static_cast<ReturnType*>(GetComponent(_entityId, _variableName));
	}

	template<typename ReturnType>
	ReturnType* ComponentTable::GetComponent(unsigned int _entityId, unsigned int _index)
	{
		return (ReturnType*)GetComponent(_entityId, _index);
	}
}

#endif