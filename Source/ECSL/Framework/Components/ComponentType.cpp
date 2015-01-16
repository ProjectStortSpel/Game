#include "ComponentType.h"

using namespace ECSL;

ComponentType::ComponentType(std::string _name,
		TableType _tableType,
		std::map<std::string, ComponentVariable>& _variables,
		std::map<unsigned int, ComponentDataType>& _offsetToTypes,
		bool _syncWithNetwork)
		: m_name(_name), m_tableType(_tableType), m_variables(_variables), m_byteSize(0), m_offsetToType(_offsetToTypes), m_syncWithNetwork(_syncWithNetwork)
{
	/* Calculate total byte size for component type */
	for (auto it = m_variables.begin(); it != m_variables.end(); ++it)
	{
		it->second.SetOffset(m_byteSize);
		m_byteSize += it->second.GetByteSize();
	}
}

ComponentType::~ComponentType()
{

}