#include "Window.h"

Window::Window(
	LPCTSTR windowTitle,
	int32_t clientWidth,
	int32_t clientHeight,
	int32_t positionX,
	int32_t positionY)
	:
	m_width(clientWidth),
	m_height(clientHeight),
	m_title(windowTitle),
	m_pWindowClass(std::make_shared<WindowAPIClass>()),
	m_pKeyboard(std::make_shared<Keyboard>()),
	m_pMouse(std::make_shared<Mouse>())
{
	MessageHandler messageHandler = std::bind(&Window::HandleMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	m_pWindowClass->SetMessageHandler(messageHandler);

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
		ErrorHandler::ErrorBox(L"Error Adjusting Window Rectangle", GetLastError(), __FILE__, __LINE__);
		throw -1;
	}

	/// Create window instance
	m_handle = CreateWindow(
		m_pWindowClass->GetName(),
		windowTitle,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRegion.right - windowRegion.left,
		windowRegion.bottom - windowRegion.top,
		nullptr,
		nullptr,
		m_pWindowClass->GetApplicationHandle(),
		m_pWindowClass.get()
	);
	if (!m_handle)
	{
		ErrorHandler::ErrorBox(L"Error creating window", GetLastError(), __FILE__, __LINE__);
		throw -1;
	}
	m_pGraphics = std::make_shared<Graphics>(m_handle);
}

Window::~Window()
{
	if (IsWindow(m_handle))
	{
		DestroyWindow(m_handle);
	}
}

void Window::Show() const noexcept
{
	if (ShowWindow(m_handle, SW_SHOW))
	{
		ErrorHandler::ErrorBox(L"Warning window was already visible", GetLastError(), __FILE__, __LINE__);
	}
}
void Window::Hide() const noexcept
{
	if (!ShowWindow(m_handle, SW_HIDE))
	{
		ErrorHandler::ErrorBox(L"Warning window was already hidden", GetLastError(), __FILE__, __LINE__);
	}
}

std::optional<uint64_t> Window::ProcessMessages() noexcept
{
	MSG message;
	while (PeekMessage(&message, nullptr, NULL, NULL, PM_REMOVE))
	{
		if (message.message == WM_QUIT)
		{
			return static_cast<uint64_t>(message.wParam);
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return std::nullopt;
}
LRESULT Window::HandleMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	/// Window Events
	{
	case WM_CLOSE:
		if (MessageBox(m_handle, L"Are you sure you want to close the app?", L"Close Window", MB_YESNOCANCEL) == IDYES)
			DestroyWindow(m_handle);
		return NULL;

	case WM_KILLFOCUS:
		m_pKeyboard->ClearKeysStates();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
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
int32_t Window::GetWidth() const noexcept
{
	return m_width;
}
int32_t Window::GetHeight() const noexcept
{
	return m_height;
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



std::shared_ptr<WindowAPIClass> Window::GetWindowClass() const noexcept
{
	return m_pWindowClass;
}