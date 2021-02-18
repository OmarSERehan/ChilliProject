#include "Window.h"

//LRESULT CALLBACK WindowProcedure
//(
//	HWND	hwnd,
//	UINT	uMsg,
//	WPARAM	wParam,
//	LPARAM	lParam
//)
//{
//	switch (uMsg)
//	{
//	// WM_DESTROY			// when the window is actually starting to close
//	// WM_NCDESTROY			// when the window is finished being closed
//	// WM_QUIT				// when the whole application should close
//	case WM_CLOSE:			// when the user tries to close the window (X button, alt+f4, etc.)
//	{
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	case WM_KEYDOWN:		// when a key has just been pressed (case insensitive)
//	{
//		if (wParam == 'F')
//			SetWindowText(hwnd, L"Respects");
//		break;
//	}
//
//	case WM_KEYUP:			// when a key has just been released
//	{
//		if (wParam == 'F')
//			SetWindowText(hwnd, L"Chilli Window");
//		break;
//	}
//
//	case WM_CHAR:			// when a character has just been pressed (case sensitive)
//	{
//		static std::wstring title;
//		title.push_back((char)wParam);
//		SetWindowText(hwnd, title.c_str());
//		break;
//	}
//
//	case WM_LBUTTONDOWN:	// when the left mouse button has just been pressed
//	{
//		POINTS point = MAKEPOINTS(lParam);
//		std::wostringstream outputStringStream;
//		outputStringStream << point.x << ", " << point.y;
//		SetWindowText(hwnd, outputStringStream.str().c_str());
//		break;
//	}
//	}
//
//	return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) 
{
	std::shared_ptr<WindowAPIClass> pWindowClass = WindowAPIClass::CreateWindowClass(L"Direct3D Window Class");
	pWindowClass->RegisterWindowClass();

	std::shared_ptr<Window> pWindow = Window::CreateWindowInstance(pWindowClass);

	// Run Message Loop
	int messageResult;
	MSG message = { NULL };
	while ((messageResult = GetMessage(&message, NULL, 0, 0)) > 0)
	{
		TranslateMessage(&message);	// Post WM_CHAR message if this message is a suitable WM_KEY_DOWN message (Among other things)
		DispatchMessage(&message);	// Pass message to the window's procedure function

		if (pWindow->GetKeyboard()->KeyIsPressed(VK_MENU))
			MessageBox(nullptr, L"Space Clicked", L"Attention", MB_OK | MB_ICONEXCLAMATION);
	}

	pWindow->DestroyWindowInstance();
	pWindowClass->UnregisterWindowClass();

	// Exit Application
	if (messageResult == -1)
	{
		return -1;
	}
	else
		{
			return message.wParam;
		}
}