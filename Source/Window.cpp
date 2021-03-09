#include "Window.h"

std::shared_ptr<Window> Window::CreateObject(
	LPCTSTR windowTitle,
	int32_t clientWidth,
	int32_t clientHeight,
	int32_t positionX,
	int32_t positionY,
	std::shared_ptr<WindowAPIClass> pWindowClass,
	std::shared_ptr<Graphics> pGraphics) noexcept
{
	/// Create window object
	std::shared_ptr<Window> pWindow = std::make_shared<Window>();

	/// Pre-Creation initialization
	if (!pWindowClass)
	{
		pWindowClass = WindowAPIClass::CreateObject();
		if (!pWindow) return nullptr;
	}
	pWindow->SetWindowClass(pWindowClass);

	MessageHandler messageHandler = std::bind(&Window::HandleMessage, pWindow.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	pWindow->GetWindowClass()->SetMessageHandler(messageHandler);

	// Create rectangle struct
	RECT windowRegion;
	{
		windowRegion.left = 100;
		windowRegion.right = windowRegion.left + clientWidth;
		windowRegion.top = 100;
		windowRegion.bottom = windowRegion.top + clientHeight;
	}

	// Add titlebar and border to window size
	BOOL result = AdjustWindowRect(&windowRegion, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	if (!result)
	{
		std::wstring errorString = std::to_wstring(GetLastError());
		MessageBox(nullptr, errorString.c_str(), L"Error adjusting window rectangle", MB_OK | MB_ICONEXCLAMATION);
		return nullptr;
	}

	/// Create window instance
	HWND windowHandle = CreateWindow(
		pWindow->GetWindowClass()->GetName(),
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
	if (!windowHandle)
	{
		std::wstring errorString = std::to_wstring(GetLastError());
		MessageBox(nullptr, errorString.c_str(), L"Error creating window", MB_OK | MB_ICONEXCLAMATION);
		return nullptr;
	}
	

	/// Post-Create window initialization
	pWindow->SetHandle(windowHandle);
	pWindow->SetWidth(clientWidth);
	pWindow->SetHeight(clientHeight);
	pWindow->SetTitle(windowTitle);
	
	pWindow->SetKeyboard(std::make_shared<Keyboard>());
	pWindow->SetMouse(std::make_shared<Mouse>());
	if (!pGraphics)
	{
		pGraphics = Graphics::CreateObject(windowHandle);
		if (!pGraphics) return nullptr;
	}
	pWindow->SetGraphics(pGraphics);


	return pWindow;
}

bool Window::DestroyObject() noexcept
{
	BOOL result = DestroyWindow(m_handle);
	if (!result)
	{
		std::wstring errorString = std::to_wstring(GetLastError());
		MessageBox(nullptr, errorString.c_str(), L"Error destroying window", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	return m_pWindowClass->DestroyObject();
}

bool Window::Show() const noexcept
{
	BOOL result = ShowWindow(m_handle, SW_SHOW);
	if (result)
	{
		std::wstring errorString = std::to_wstring(GetLastError());
		MessageBox(nullptr, errorString.c_str(), L"Warning window was already visible", MB_OK | MB_ICONWARNING);
	}

	return true;
}
bool Window::Hide() const noexcept
{
	BOOL result = ShowWindow(m_handle, SW_HIDE);
	if (!result)
	{
		std::wstring errorString = std::to_wstring(GetLastError());
		MessageBox(nullptr, errorString.c_str(), L"Warning window was already hidden", MB_OK | MB_ICONWARNING);
		return false;
	}

	return true;
}

std::optional<int32_t> Window::ProcessMessages() noexcept
{
	MSG message;

	while (PeekMessage(&message, nullptr, NULL, NULL, PM_REMOVE))
	{
		if (message.message == WM_QUIT)
		{
			return static_cast<int32_t>(message.wParam);
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return {};
}
LRESULT Window::HandleMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	/// Window Events
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KILLFOCUS:
		m_pKeyboard->ClearKeysStates();
		break;
	}


	/// Keyboard Events
	{
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


	/// Mouse Events
	{
	case WM_MOUSEMOVE:
	{
		const POINTS points = MAKEPOINTS(lParam);
		bool bInClientRegion = 0 <= points.x && points.x < m_width && 0 <= points.y && points.y < m_height;
		
		if (bInClientRegion)
		{
			m_pMouse->OnMouseMove(points.x, points.y);
			if (!m_pMouse->IsInWindow())
			{
				SetCapture(m_handle);
				m_pMouse->OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				m_pMouse->OnMouseMove(points.x, points.y);
			}
			else
			{
				ReleaseCapture();
				m_pMouse->OnMouseLeave();
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		const POINTS points = MAKEPOINTS(lParam);
		m_pMouse->OnLeftPressed(points.x, points.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lParam);
		m_pMouse->OnLeftReleased(points.x, points.y);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		const POINTS points = MAKEPOINTS(lParam);
		m_pMouse->OnRightPressed(points.x, points.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lParam);
		m_pMouse->OnRightReleased(points.x, points.y);
		break;
	}

	case WM_MOUSEWHEEL:
	{
		const POINTS points = MAKEPOINTS(lParam);
		const int32_t delta = GET_WHEEL_DELTA_WPARAM(wParam);
		m_pMouse->OnWheelDelta(points.x, points.y, delta);
		break;
	}
	}
	}

	return DefWindowProc(windowHandle, message, wParam, lParam);
}

std::shared_ptr<Keyboard> Window::GetKeyboard() const noexcept
{
	return m_pKeyboard;
}
std::shared_ptr<Mouse> Window::GetMouse() const noexcept
{
	return m_pMouse;
}
std::shared_ptr<Graphics> Window::GetGraphics() const noexcept
{
	return m_pGraphics;
}


bool Window::SetTitle(const std::wstring& title) noexcept
{	
	BOOL result = SetWindowText(m_handle, title.c_str());
	if (!result)
	{
		std::wstring errorString = std::to_wstring(GetLastError());
		MessageBox(nullptr, errorString.c_str(), L"Error setting window title", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	m_title = title.c_str(); // TODO: check this shit if it works
	return true;
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
void Window::SetWidth(int32_t width) noexcept
{
	m_width = width;
}
void Window::SetHeight(int32_t height) noexcept
{
	m_height = height;
}
void Window::SetKeyboard(std::shared_ptr<Keyboard> pKeyboard) noexcept
{
	m_pKeyboard = pKeyboard;
}
void Window::SetMouse(std::shared_ptr<Mouse> pMouse) noexcept
{
	m_pMouse = pMouse;
}
void Window::SetGraphics(std::shared_ptr<Graphics> pGraphics) noexcept
{
	m_pGraphics = pGraphics;
}