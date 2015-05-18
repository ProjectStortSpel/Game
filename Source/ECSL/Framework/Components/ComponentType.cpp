#include "ComponentType.h"

using namespace ECSL;

ComponentType::ComponentType(const std::string& _name,
		TableType _tableType,
		std::map<std::string, ComponentVariable>& _variables,
		std::map<unsigned int, ComponentDataType>& _offsetToTypes,
		bool _syncWithNetwork)
		: m_name(new std::string(_name)), m_tableType(_tableType), m_variables(new std::map<std::string, ComponentVariable>(_variables)), m_byteSize(0), m_offsetToType(new std::map<unsigned int, ComponentDataType>(_offsetToTypes)), m_syncWithNetwork(_syncWithNetwork)
{
	/* Calculate total byte size for component type */
	for (auto it = m_variables->begin(); it != m_variables->end(); ++it)
	{
		//it->second.SetOffset(m_byteSize);
		m_byteSize += it->second.GetByteSize();
	}
}

ComponentType::~ComponentType()
{
	delete(m_name);
	delete(m_variables);
	delete(m_offsetToType);
}