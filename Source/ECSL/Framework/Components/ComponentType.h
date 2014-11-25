#ifndef COMPONENTTYPE_H
#define COMPONENTTYPE_H

#include <SDL/SDL.h>
#include <string>

namespace ECSL
{
	/*class ComponentSetting
	{
	private:
		std::string	m_name;
		std::string	m_value;

	public:
		ComponentSetting(std::string _name, std::string _value) : m_name(_name), m_value(_value) { }
		~ComponentSetting();

		inline const std::string& GetName() const	{ return m_name; }
		inline const std::string& GetValue() const	{ return m_value; }
	};

	class ComponentVariable 
	{
	private:
		std::string	m_name;
		int m_byteSize;

	public:
		ComponentVariable(std::string _name, int _byteSize) : m_name(_name), m_byteSize(_byteSize) { }
		~ComponentVariable();

		inline const std::string& GetName() const	{ return m_name; }
		inline const int GetByteSize() const		{ return m_byteSize; }
	};

	class ComponentType
	{
	private:
		std::string	m_name;
		std::vector<ComponentSetting>* m_settings;
		std::vector<ComponentVariable>* m_variables;
		std::vector<std::string>* m_debugStrings;
	public:
		ComponentType(
			std::string	_name,
			std::vector<ComponentSetting>* _settings,
			std::vector<ComponentVariable>* _variables,
			std::vector<std::string>* _debugStrings);
		~ComponentType();

		inline const std::string& GetName() const							{ return m_name; }
		inline const std::vector<ComponentSetting>& GetSettings() const		{ return *m_settings; }
		inline const std::vector<ComponentVariable>& GetVariables() const	{ return *m_variables; }
		inline const std::vector<std::string>& GetDebugStrings() const		{ return *m_debugStrings; }
	};*/
	enum DECLSPEC TableType
	{
		Array,
		Map
	};

	class DECLSPEC ComponentTypeInterface
	{
	public:
		virtual const int GetByteSize() const = 0;
		virtual const int GetIndexFor(std::string _var) const = 0;
		virtual const TableType GetTableType() const = 0;
		virtual const int GetNumberOfRows() const = 0;
	};
}

#endif