#include "ErrorHandler.h"

std::string ErrorHandler::GetErrorDescription(DWORD errorCode) noexcept
{
	WCHAR errorDescription[512];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, NULL, errorDescription, 512, NULL);

	return ConvertToNarrowString(errorDescription);
}
std::string ErrorHandler::GetErrorDescription(HRESULT result) noexcept
{
	return std::system_category().message(result);
}
std::string ErrorHandler::GetErrorDescription(std::vector<std::string> errorMessages) noexcept
{
	std::string fullDescription = "Errors(" + std::to_string(errorMessages.size()) + "):\n";

	for (auto& message : errorMessages)
		fullDescription += message + "\n\n";

	return fullDescription;
}


void ErrorHandler::ErrorBox(const wchar_t* title, const wchar_t* description, const char* file, uint32_t line) noexcept
{
	std::wstring fullDescription = FormatErrorDescription(description, file, line);

	MessageBox(nullptr, title, fullDescription.c_str(), MB_OK | MB_ICONEXCLAMATION);
}
void ErrorHandler::ErrorBox(const wchar_t* title, DWORD errorCode, const char* file, uint32_t line) noexcept
{
	WCHAR errorDescription[512];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errorCode, NULL, errorDescription, 512, NULL);

	std::wstring fullDescription = FormatErrorDescription(errorDescription, file, line);
	
	MessageBox(nullptr, fullDescription.c_str(), title, MB_OK | MB_ICONEXCLAMATION);
}
void ErrorHandler::ErrorBox(const wchar_t* title, HRESULT result, const char* file, uint32_t line) noexcept
{
	std::string errorDescription = std::system_category().message(result);
	std::wstring fullDescription = FormatErrorDescription(ConvertToWideString(errorDescription), file, line);
	
	MessageBox(nullptr, fullDescription.c_str(), title, MB_OK | MB_ICONEXCLAMATION);
}
void ErrorHandler::ErrorBox(const wchar_t* title, std::vector<std::string> errorMessages, const char* file, uint32_t line) noexcept
{
	std::wstring fullDescription = FormatErrorDescription(L"", file, line);

	for (auto& message : errorMessages)
		fullDescription += ConvertToWideString(message) + L"\n\n";

	MessageBox(nullptr, fullDescription.c_str(), title, MB_OK | MB_ICONEXCLAMATION);
}


std::wstring ErrorHandler::ConvertToWideString(const std::string& str) noexcept
{
	int numOfChars = MultiByteToWideChar(CP_ACP, NULL, str.c_str(), -1, NULL, NULL);
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), numOfChars, buffer, 512);
	
	return std::wstring(buffer);
}
std::string ErrorHandler::ConvertToNarrowString(const std::wstring& wstr) noexcept
{
	int numOfChars = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
	
	std::string str(numOfChars, NULL);
	WideCharToMultiByte(CP_UTF8, NULL, wstr.c_str(), (int)wstr.size(), &str[0], 512, NULL, NULL);

	return str;
}

std::wstring ErrorHandler::FormatErrorDescription(const std::wstring& description, const char* file, uint32_t line) noexcept
{
	std::wstringstream fullDescription;
	fullDescription << 
		L"[FILE] " << file << L'\n' <<
		L"[LINE] " << line << L'\n' <<
		L"[DESC] " << description << L'\n';

	return fullDescription.str();
}