#include "Window.h"

std::shared_ptr<Window> Window::CreateWindowInstance(
	std::shared_ptr<WindowAPIClass> pWindowClass,
	LPCTSTR windowTitle,
	uint32_t clientWidth,
	uint32_t clientHeight,
	uint32_t positionX,
	uint32_t positionY
) noexcept
{
	// Create window object
	std::shared_ptr<Window> pWindow = std::make_shared<Window>();
	{
		pWindow->SetWindowClass(pWindowClass);

		pWindow->SetWidth(clientWidth);
		pWindow->SetHeight(clientHeight);

		pWindow->SetKeyboard(std::make_shared<Keyboard>());
	}

	// Create rectangle struct
	RECT windowRegion;
	{
		windowRegion.left =		100;
		windowRegion.right =	windowRegion.left + clientWidth;
		windowRegion.top =		100;
		windowRegion.bottom =	windowRegion.top + clientHeight;
	}

	// Add titlebar and border to window size
	{
		BOOL result = AdjustWindowRect(&windowRegion, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
		if (!result)
		{
			std::wstring errorString = std::to_wstring(GetLastError());
			MessageBox(nullptr, errorString.c_str(), L"Error adjusting window rectangle", MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}
	}

	// Create the window
	{
		HWND windowHandle = CreateWindow(
			pWindow->GetWindowClass()->GetWindowClassName(),
			windowTitle,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRegion.right - windowRegion.left,
			windowRegion.bottom - windowRegion.top,
			nullptr,
			nullptr,
			pWindow->GetWindowClass()->GetApplicationHandle(),
			pWindow->GetWindowClass().get()
		);
		if (!pWindow)
		{
			std::wstring errorString = std::to_wstring(GetLastError());
			MessageBox(nullptr, errorString.c_str(), L"Error creating window", MB_OK | MB_ICONEXCLAMATION);
			return nullptr;
		}
		pWindow->SetHandle(windowHandle);

		//MessageHandler messageHandler = [&](HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) { return pWindow->HandleMessage(windowHandle, message, wParam, lParam); };
		MessageHandler messageHandler = std::bind(&Window::HandleMessage, pWindow.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		pWindow->GetWindowClass()->SetMessageHandler(messageHandler);
	}

	// Show the window
	{
		ShowWindow(pWindow->m_handle, SW_SHOWDEFAULT);
	}

	return pWindow;
}

void Window::DestroyWindowInstance() const noexcept
{
	DestroyWindow(m_handle);
}

LRESULT Window::HandleMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	/// Window Events
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KILLFOCUS:
		m_pKeyboard->ClearKeysStates();
		break;
	

	/// Keyboard Events
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (m_pKeyboard->AutoRepeatIsEnabled() || !(lParam & (1 << 30)))
			m_pKeyboard->OnKeyPressed(static_cast<unsigned char>(wParam));
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		m_pKeyboard->OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		m_pKeyboard->OnChar(static_cast<unsigned char>(wParam));
		break;
	}

	return DefWindowProc(windowHandle, message, wParam, lParam);
}

std::shared_ptr<Keyboard> Window::GetKeyboard() const noexcept
{
	return m_pKeyboard;
}


/// Private Functions
void Window::SetWindowClass(std::shared_ptr<WindowAPIClass> pWindowClass) noexcept
{
	m_pWindowClass = pWindowClass;
}
std::shared_ptr<WindowAPIClass> Window::GetWindowClass() const noexcept
{
	return m_pWindowClass;
}

void Window::SetHandle(HWND handle) noexcept
{
	m_handle = handle;
}
void Window::SetWidth(uint32_t width) noexcept
{
	m_width = width;
}
void Window::SetHeight(uint32_t height) noexcept
{
	m_height = height;
}
void Window::SetKeyboard(std::shared_ptr<Keyboard> pKeyboard) noexcept
{
	m_pKeyboard = pKeyboard;
}