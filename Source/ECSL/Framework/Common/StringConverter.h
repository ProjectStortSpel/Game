#ifndef STRINGCONVERTER_H
#define STRINGCONVERTER_H
#ifdef _WINDOWS

#include <vector>

namespace ECSL
{
	namespace StringConverter
	{
		std::wstring StringToWString(const std::string& _s);
		std::string WStringToString(const std::wstring& _s);
	}
}

#endif
#endif