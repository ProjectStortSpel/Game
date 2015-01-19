#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

#include <inttypes.h>
#include <map>
#include <vector>
#include <string>
#include <SDL/SDL.h>

namespace ECSL
{
	#define	CHARSIZE 32

	enum TableType
	{
		None,
		Array,
		Map
	};

	enum ComponentDataType
	{
		INT, FLOAT, TEXT, BOOL,
		MATRIX, REFERENCE, INT64
	};

	static int GetByteSizeFromType(ComponentDataType _type)
	{
		switch (_type)
		{
		case ComponentDataType::INT64:
			return sizeof(uint64_t);
			break;
		case ComponentDataType::INT:
			return sizeof(int);
			break;
		case ComponentDataType::FLOAT:
			return sizeof(float);
			break;

		case ComponentDataType::TEXT:
			return CHARSIZE*sizeof(char);
			break;
		case ComponentDataType::MATRIX:
			return 64;
			break;

		case ComponentDataType::REFERENCE:
			return sizeof(int);
			break;

		case ComponentDataType::BOOL:
			return sizeof(bool);
			break;
		}
		return 0;
	}

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
		bool m_syncWithNetwork;
		TableType m_tableType;
		unsigned int m_byteSize;
		std::map<std::string, ComponentVariable> m_variables;
		std::map<unsigned int, ComponentDataType> m_offsetToType;
	public:
		ComponentType(
			std::string	_name,
			TableType _tableType,
			std::map<std::string, ComponentVariable>& _variables,
			std::map<unsigned int, ComponentDataType>& _offsetToTypes,
			bool _syncWithNetwork = true);
		~ComponentType();

		inline const std::string& GetName() const									{ return m_name; }
		inline const bool GetNetworkSyncState() const								{ return m_syncWithNetwork; }
		inline const TableType GetTableType() const									{ return m_tableType; }
		inline const int GetByteSize() const										{ return m_byteSize; }
		inline const std::map<std::string, ComponentVariable>* GetVariables() const	{ return &m_variables; }
		inline const std::map<unsigned int, ComponentDataType>* GetDataTypes() const { return &m_offsetToType; }
	};
}

#endif