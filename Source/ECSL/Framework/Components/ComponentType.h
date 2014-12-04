#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

#include <map>
#include <vector>
#include <string>
#include <SDL/SDL.h>

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
		unsigned int m_offset;
		unsigned int m_byteSize;

	public:
		ComponentVariable(std::string _name, unsigned int _byteSize) : m_name(_name), m_byteSize(_byteSize) { }
		~ComponentVariable() { }

		inline const std::string& GetName() const		{ return m_name; }
		inline const unsigned int GetOffset() const		{ return m_offset; }
		inline const unsigned int GetByteSize() const	{ return m_byteSize; }

		inline void SetOffset(unsigned int _offset) { m_offset = _offset; }
	};

	class DECLSPEC ComponentType
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