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
	static std::shared_ptr<WindowAPIClass> CreateObject(LPCWSTR className = L"Simple Win32 Window Class", MessageHandler messageHandler = DefWindowProc) noexcept;
	bool DestroyObject() const noexcept;

	// Getter functions
	HINSTANCE GetApplicationHandle() const noexcept;
	LPCWSTR GetName() const noexcept;
	
	// Setter functions
	void SetName(LPCWSTR className) noexcept;
	void SetApplicationHandle(HINSTANCE applicationHandle) noexcept;
	void SetMessageHandler(MessageHandler messageHandler) noexcept;

	// Message handling function
	static LRESULT CALLBACK HandleMessageEntryPoint(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept;


private:
	HINSTANCE m_applicationHandle = NULL;
	LPCWSTR m_pName = nullptr;
	MessageHandler m_messageHandler;



public:
	// Constructor and destructor
	WindowAPIClass() = default;
	~WindowAPIClass() = default;

	// Copy semantics
	WindowAPIClass(const WindowAPIClass&) = delete;
	WindowAPIClass& operator = (const WindowAPIClass&) = delete;

	// Move sematics
	WindowAPIClass(WindowAPIClass&&) = delete;
	WindowAPIClass& operator = (WindowAPIClass&&) = delete;
};

