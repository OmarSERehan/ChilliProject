#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <system_error>

#include "WinHeader.h"

class PopUp
{
public:
	static void ErrorBox(const wchar_t* title, const wchar_t* description, const char* file, uint32_t line) noexcept;
	static void ErrorBox(const wchar_t* title, DWORD description, const char* file, uint32_t line) noexcept;
	static void ErrorBox(const wchar_t* title, HRESULT result, const char* file, uint32_t line) noexcept;
	static void ErrorBox(const wchar_t* title, std::vector<std::string> errorMessages, const char* file, uint32_t line) noexcept;

private:
	static std::wstring ConvertToWideString(const std::string& str) noexcept;
	static std::wstring FormatErrorDescription(const std::wstring& description, const char* file, uint32_t line) noexcept;
};

