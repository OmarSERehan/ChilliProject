#include "WindowAPIClass.h"

WindowAPIClass::WindowAPIClass(LPCWSTR className, MessageHandler messageHandler)
	:
	m_applicationHandle(GetModuleHandle(nullptr)),
	m_pName(className),
	m_messageHandler(messageHandler)
{
	WNDCLASS WindowClassStruct = { NULL };

	WindowClassStruct.hInstance = m_applicationHandle;
	WindowClassStruct.lpszClassName = m_pName;
	WindowClassStruct.lpszMenuName = nullptr;
	WindowClassStruct.lpfnWndProc = HandleMessageEntryPoint;
	WindowClassStruct.style = CS_OWNDC;

	WindowClassStruct.cbClsExtra = 0;
	WindowClassStruct.cbWndExtra = 0;
	WindowClassStruct.hIcon = static_cast<HICON>(LoadImage(m_applicationHandle, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 32, 32, 0));
	WindowClassStruct.hCursor = nullptr;
	WindowClassStruct.hbrBackground = nullptr;

	ATOM result = RegisterClass(&WindowClassStruct);
	if (!result)
	{
		std::wstring errorString = std::to_wstring(GetLastError());
		MessageBox(nullptr, errorString.c_str(), L"Error registering window class", MB_OK | MB_ICONEXCLAMATION);
		throw - 1;
	}
}
WindowAPIClass::~WindowAPIClass()
{
	if (!UnregisterClass(m_pName, m_applicationHandle))
	{
		ErrorHandler::ErrorBox(L"Error Unregistering Window Class from WIN32 API", GetLastError(), __FILE__, __LINE__);
		throw - 1;
	}
}


HINSTANCE WindowAPIClass::GetApplicationHandle() const noexcept
{
	return m_applicationHandle;
}

LPCTSTR WindowAPIClass::GetName() const noexcept
{
	return m_pName;
}

LRESULT CALLBACK WindowAPIClass::HandleMessageEntryPoint(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
	WindowAPIClass* pWindowClass = nullptr;

	if (message == WM_NCCREATE)
	{
		// extract pointer to api window class instance that owns this message
		const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		pWindowClass = static_cast<WindowAPIClass*>(pCreate->lpCreateParams);

		// set Win32 API user data to store pointer to our api window class instance
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindowClass));

		//SetWindowLongPtr(windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&APIClass::HandleMessage));
	}
	else
	{
		pWindowClass = reinterpret_cast<WindowAPIClass*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
	}

	if (pWindowClass)
		return pWindowClass->m_messageHandler(windowHandle, message, wParam, lParam);
	return DefWindowProc(windowHandle, message, wParam, lParam);
}

void WindowAPIClass::SetMessageHandler(MessageHandler messageHandler) noexcept
{
	m_messageHandler = messageHandler;
}