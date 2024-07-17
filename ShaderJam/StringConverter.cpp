#include "StringConverter.h"

std::wstring StringConverter::StringToWide(std::string str) 
{
	std::wstring wstring(str.begin(), str.end());
	return wstring;
}