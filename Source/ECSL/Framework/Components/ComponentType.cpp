#include "ComponentType.h"

using namespace ECSL;

//ComponentType::ComponentType(int _byteSize, TableType _tableType, std::vector<std::pair<std::string, int>> _variables)
//	: m_byteSize(_byteSize), m_tableType(_tableType)
//{
//	m_variableIndices = new std::map<std::string, int>();
//	for (int n = 0; n < _variables.size(); ++n)
//	{
//		std::string tName = _variables[n].first;
//		int tValue = _variables[n].second;
//
//		m_variableIndices->operator[]("tName") = tValue;
//	}
//}
//
//ComponentType::~ComponentType()
//{
//
//}