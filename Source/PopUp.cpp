#include "PopUp.h"

void PopUp::ErrorBox(const wchar_t* title, const wchar_t* description, const char* file, uint32_t line) noexcept
{
	std::wstring fullDescription = FormatErrorDescription(description, file, line);

	MessageBox(nullptr, title, fullDescription.c_str(), MB_OK | MB_ICONEXCLAMATION);
}

void PopUp::ErrorBox(const wchar_t* title, DWORD errorCode, const char* file, uint32_t line) noexcept
{
	WCHAR errorDescription[512];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, NULL, errorDescription, 512, NULL);

	std::wstring fullDescription = FormatErrorDescription(errorDescription, file, line);
	
	MessageBox(nullptr, fullDescription.c_str(), title, MB_OK | MB_ICONEXCLAMATION);
}

void PopUp::ErrorBox(const wchar_t* title, HRESULT result, const char* file, uint32_t line) noexcept
{
	std::string errorDescription = std::system_category().message(result);
	std::wstring fullDescription = FormatErrorDescription(ConvertToWideString(errorDescription), file, line);
	
	MessageBox(nullptr, fullDescription.c_str(), title, MB_OK | MB_ICONEXCLAMATION);
}

void PopUp::ErrorBox(const wchar_t* title, std::vector<std::string> errorMessages, const char* file, uint32_t line) noexcept
{
	std::wstring fullDescription = FormatErrorDescription(L"", file, line);

	for (auto& message : errorMessages)
		fullDescription += ConvertToWideString(message) + L"\n\n";

	MessageBox(nullptr, fullDescription.c_str(), title, MB_OK | MB_ICONEXCLAMATION);
}


std::wstring PopUp::ConvertToWideString(const std::string& str) noexcept
{
	int numOfChars = MultiByteToWideChar(CP_ACP, NULL, str.c_str(), -1, NULL, NULL);
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), numOfChars, buffer, 512);
	
	return std::wstring(buffer);
}

std::wstring PopUp::FormatErrorDescription(const std::wstring& description, const char* file, uint32_t line) noexcept
{
	std::wstringstream fullDescription;
	fullDescription << 
		L"[LINE] " << line << L'\n' << 
		L"[FILE] " << file << L'\n' << 
		L"[DESC] " << description << L'\n';

	return fullDescription.str();
}