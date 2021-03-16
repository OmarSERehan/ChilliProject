#pragma once
#include <sstream>
#include <utility>
#include <functional>

#include "WinHeader.h"
#include "ErrorHandler.h"
#include "../resource.h"


typedef std::function<LRESULT(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)> MessageHandler;

class WindowAPIClass
{
public:
	WindowAPIClass(LPCWSTR className = L"Simple Win32 Window Class", MessageHandler messageHandler = DefWindowProc);
	~WindowAPIClass();

	// Message handling function
	static LRESULT CALLBACK HandleMessageEntryPoint(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept;

	void SetMessageHandler(MessageHandler messageHandler) noexcept;

	LPCWSTR GetName() const noexcept;
	HINSTANCE GetApplicationHandle() const noexcept;

private:
	HINSTANCE m_applicationHandle = NULL;
	LPCWSTR m_pName = nullptr;
	MessageHandler m_messageHandler;


public:
	// Copy semantics
	WindowAPIClass(const WindowAPIClass&) = delete;
	WindowAPIClass& operator = (const WindowAPIClass&) = delete;

	// Move sematics
	WindowAPIClass(WindowAPIClass&&) = delete;
	WindowAPIClass& operator = (WindowAPIClass&&) = delete;
};

