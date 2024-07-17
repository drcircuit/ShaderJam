#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(std::string message) {
	std::string errmsg = "Error: " + message;
	MessageBoxA(NULL, errmsg.c_str(), "Error", MB_ICONERROR);
}

void ErrorLogger::Log(HRESULT hr, std::string message) {
	_com_error error(hr);
	std::wstring errmsg = L"Error: " + StringConverter::StringToWide(message) + L"\n"+error.ErrorMessage();
	MessageBoxW(NULL, errmsg.c_str(), L"Error", MB_ICONERROR);
}