#include "StringConverter.h"
#ifdef _WINDOWS

#include <Windows.h>

using namespace ECSL;

std::wstring StringConverter::StringToWString(const std::string& _s)
{
	int len;
	int slength = (int)_s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, _s.c_str(), slength, 0, 0);
	std::wstring r(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, _s.c_str(), slength, &r[0], len);
	return r;
}

std::string StringConverter::WStringToString(const std::wstring& _s)
{
	int len;
	int slength = (int)_s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, _s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, _s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

#endif