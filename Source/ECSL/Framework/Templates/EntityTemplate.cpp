#include "EntityTemplate.h"
using namespace ECSL;

EntityTemplate::EntityTemplate(std::string	_name, std::map<std::string, std::vector<TemplateEntry>>& _components) :
m_name(_name), m_components(_components)
{

}
EntityTemplate::~EntityTemplate()
{

}