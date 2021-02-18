#include "WindowClass.h"

std::shared_ptr<WindowAPIClass> WindowAPIClass::CreateWindowClass(LPCWSTR className, MessageHandler messageHandler) noexcept
{
	std::shared_ptr<WindowAPIClass> pWindowClass = std::make_shared<WindowAPIClass>();
	
	pWindowClass->SetApplicationHandle(GetModuleHandle(nullptr));
	pWindowClass->SetWindowClassName(className);
	pWindowClass->SetMessageHandler(messageHandler);

	return pWindowClass;
}


bool WindowAPIClass::RegisterWindowClass() const noexcept
{
	WNDCLASS WindowClassStruct = { NULL };

	WindowClassStruct.hInstance = m_applicationHandle;
	WindowClassStruct.lpszClassName = m_className;
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
		return false;
	}

	return true;
}

bool WindowAPIClass::UnregisterWindowClass() const noexcept
{
	ATOM result = UnregisterClass(m_className, m_applicationHandle);
	if (!result)
	{
		std::wstring errorCode = std::to_wstring(GetLastError());
		MessageBox(nullptr, errorCode.c_str(), L"Error unregistering window class", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}


HINSTANCE WindowAPIClass::GetApplicationHandle() const noexcept
{
	return m_applicationHandle;
}

LPCTSTR WindowAPIClass::GetWindowClassName() const noexcept
{
	return m_className;
}


void WindowAPIClass::SetApplicationHandle(HINSTANCE applicationHandle) noexcept
{
	m_applicationHandle = applicationHandle;
}

void WindowAPIClass::SetWindowClassName(LPCWSTR className) noexcept
{
	m_className = className;
}

void WindowAPIClass::SetMessageHandler(MessageHandler messageHandler) noexcept
{
	m_messageHandler = messageHandler;
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