#ifndef ENTITYTEMPLATE_H
#define ENTITYTEMPLATE_H
#include <vector>
#include <map>
#include <string>
#include "../Components/ComponentType.h"
namespace ECSL
{
	struct TemplateEntry
	{
		TemplateEntry(int data){ m_intData = data; m_dataType = ComponentDataType::INT; }
		TemplateEntry(float data){ m_floatData = data; m_dataType = ComponentDataType::FLOAT; }
		TemplateEntry(std::string data){ m_textData = data; m_dataType = ComponentDataType::CHAR; }

		ComponentDataType GetDataType(){ return m_dataType; }
		int GetIntData(){ return m_intData; }
		float GetFloatData(){ return m_floatData; }
		std::string GetTextData(){ return m_textData; }

	private:
		ComponentDataType m_dataType;
		std::string m_textData;
		int m_intData;
		float m_floatData;
	};

	class EntityTemplate
	{
	public:
		EntityTemplate(
			std::string	_name,
			std::map<std::string, std::vector<TemplateEntry>>& _components
			);
		~EntityTemplate();

		inline std::string& GetName(){ return m_name; }
		inline std::map<std::string, std::vector<TemplateEntry>>* GetComponents() { return &m_components; }

	private:
		std::string	m_name;
		std::map<std::string, std::vector<TemplateEntry>> m_components;
	};
}



#endif