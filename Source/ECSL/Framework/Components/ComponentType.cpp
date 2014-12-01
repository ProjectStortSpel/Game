#include "ComponentType.h"

using namespace ECSL;

ComponentType::ComponentType(std::string _name,
		TableType _tableType,
		std::map<std::string, ComponentVariable>& _variables)
		: m_name(_name), m_tableType(_tableType), m_byteSize(0)
{
	m_variables = _variables;

	/* Calculate total byte size for component type */
	for (auto it = _variables.begin(); it != _variables.end(); ++it)
		m_byteSize += it->second.GetByteSize();
}

ComponentType::~ComponentType()
{

}