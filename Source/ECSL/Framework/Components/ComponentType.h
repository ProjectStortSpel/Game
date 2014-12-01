#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

#include <map>
#include <vector>
#include <string>

namespace ECSL
{
	enum TableType
	{
		None,
		Array,
		Map
	};

	class ComponentVariable 
	{
	private:
		std::string	m_name;
		int m_byteSize;

	public:
		ComponentVariable(std::string _name, int _byteSize) : m_name(_name), m_byteSize(_byteSize) { }
		~ComponentVariable() { }

		inline const std::string& GetName() const	{ return m_name; }
		inline const int GetByteSize() const		{ return m_byteSize; }
	};

	class ComponentType
	{
	private:
		std::string	m_name;
		TableType m_tableType;
		unsigned int m_byteSize;
		std::map<std::string, ComponentVariable> m_variables;
	public:
		ComponentType(
			std::string	_name,
			TableType _tableType,
			std::map<std::string, ComponentVariable>& _variables);
		~ComponentType();

		inline const std::string& GetName() const									{ return m_name; }
		inline const TableType GetTableType() const									{ return m_tableType; }
		inline const int GetByteSize() const										{ return m_byteSize; }
		inline const std::map<std::string, ComponentVariable>& GetVariables() const	{ return m_variables; }
	};
}

#endif