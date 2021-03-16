#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <system_error>

#include "WinHeader.h"


class ErrorHandler
{
public:
	static std::string GetErrorDescription(DWORD errorCode) noexcept;
	static std::string GetErrorDescription(HRESULT result) noexcept;
	static std::string GetErrorDescription(std::vector<std::string> errorMessages) noexcept;

	static void ErrorBox(const wchar_t* title, const wchar_t* description, const char* file, uint32_t line) noexcept;
	static void ErrorBox(const wchar_t* title, DWORD errorCode, const char* file, uint32_t line) noexcept;
	static void ErrorBox(const wchar_t* title, HRESULT result, const char* file, uint32_t line) noexcept;
	static void ErrorBox(const wchar_t* title, std::vector<std::string> errorMessages, const char* file, uint32_t line) noexcept;

	static void Log(const std::string& message) noexcept;

private:
	static std::wstring ConvertToWideString(const std::string& str) noexcept;
	static std::string ConvertToNarrowString(const std::wstring& wstr) noexcept;

	static std::wstring FormatErrorDescription(const std::wstring& description, const char* file, uint32_t line) noexcept;
};

