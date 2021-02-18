#pragma once
#include <sstream>
#include <utility>
#include <functional>

#include "WinHeader.h"
#include "../resource.h"


typedef std::function<LRESULT(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)> MessageHandler;

class WindowAPIClass
{
public:
	// Creates a new window class
	static std::shared_ptr<WindowAPIClass> CreateWindowClass(LPCWSTR className = L"Simple Win32 Window Class", MessageHandler messageHandler = DefWindowProc) noexcept;

	// Registers the window class to the Win32 API
	bool RegisterWindowClass() const noexcept;

	// Unregisters the window class from the Win32 API
	bool UnregisterWindowClass() const noexcept;

	// Getter functions
	HINSTANCE GetApplicationHandle() const noexcept;
	LPCWSTR GetWindowClassName() const noexcept;
	
	// Setter functions
	void SetWindowClassName(LPCWSTR className) noexcept;
	void SetApplicationHandle(HINSTANCE applicationHandle) noexcept;
	void SetMessageHandler(MessageHandler messageHandler) noexcept;

	// Message handling function
	static LRESULT CALLBACK HandleMessageEntryPoint(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
	

	// Constructor and destructor
	WindowAPIClass() = default;
	~WindowAPIClass() = default;

	// Copy semantics
	WindowAPIClass(const WindowAPIClass&) = delete;
	WindowAPIClass& operator = (const WindowAPIClass&) = delete;

	// Move sematics
	WindowAPIClass(WindowAPIClass&&) = delete;
	WindowAPIClass& operator = (WindowAPIClass&&) = delete;


private:
	HINSTANCE m_applicationHandle = NULL;
	LPCWSTR m_className = nullptr;
	MessageHandler m_messageHandler;
};

