#include "ComponentType.h"

using namespace ECSL;

ComponentType::ComponentType(std::string _name,
	std::vector<ComponentSetting>* _settings,
	std::vector<ComponentVariable>* _variables,
	std::vector<std::string>* _debugStrings)
	: m_name(_name), m_settings(_settings), m_variables(_variables), m_debugStrings(_debugStrings)
{

}

ComponentType::~ComponentType()
{

}