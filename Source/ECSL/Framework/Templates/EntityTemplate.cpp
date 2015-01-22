#include "EntityTemplate.h"
using namespace ECSL;

EntityTemplate::EntityTemplate(const std::string& _name, std::map<std::string, std::vector<TemplateEntry*>>& _components) :
m_name(new std::string(_name)), m_components(new std::map<std::string, std::vector<TemplateEntry*>>(_components))
{

}
EntityTemplate::~EntityTemplate()
{
	delete(m_name);
	delete(m_components);
}

TemplateEntry::TemplateEntry(int _data)
{ 
	m_intData = _data; 
	m_dataType = ComponentDataType::INT; 
}

TemplateEntry::TemplateEntry(float _data) 
{ 
	m_floatData = _data; 
	m_dataType = ComponentDataType::FLOAT; 
}

TemplateEntry::TemplateEntry(const std::string& _data)
{
	m_textData = _data;
	m_dataType = ComponentDataType::TEXT; 
}

TemplateEntry::TemplateEntry(bool _data)
{ 
	m_boolData = _data; 
	m_dataType = ComponentDataType::BOOL; 
}

TemplateEntry::~TemplateEntry()
{

}