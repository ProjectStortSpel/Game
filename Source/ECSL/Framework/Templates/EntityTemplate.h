#ifndef ENTITYTEMPLATE_H
#define ENTITYTEMPLATE_H
#include <vector>
#include <map>
#include <string>
#include "../Components/ComponentType.h"
namespace ECSL
{
	struct DECLSPEC TemplateEntry
	{
		TemplateEntry(int _data);
		TemplateEntry(float _data);
		TemplateEntry(std::string& _data);
		TemplateEntry(bool _data);

		~TemplateEntry();

		ComponentDataType GetDataType(){ return m_dataType; }
		int GetIntData(){ return m_intData; }
		float GetFloatData(){ return m_floatData; }
		const std::string& GetTextData(){ return m_textData; }
		bool GetBoolData() { return m_boolData; }

	private:
		ComponentDataType m_dataType;
		std::string m_textData;
		int m_intData;
		float m_floatData;
		bool m_boolData;
	};

	class DECLSPEC EntityTemplate
	{
	public:
		EntityTemplate(
			const std::string& _name,
			std::map<std::string, std::vector<TemplateEntry*>>& _components
			);
		~EntityTemplate();

		inline const std::string& GetName(){ return *m_name; }
		inline std::map<std::string, std::vector<TemplateEntry*>>* GetComponents() { return m_components; }

	private:
		std::string* m_name;
		std::map<std::string, std::vector<TemplateEntry*>>* m_components;
	};
}



#endif