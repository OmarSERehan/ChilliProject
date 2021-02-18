#pragma once
#include <utility>
#include <iostream>
#include <functional>

#include "WindowClass.h"
#include "Keyboard.h"

class Window
{
public:
	static std::shared_ptr<Window> CreateWindowInstance(
		std::shared_ptr<WindowAPIClass> pWindowClass,
		LPCTSTR windowTitle = L"Window Title",
		uint32_t clientWidth = 640,
		uint32_t clientHeight = 320,
		uint32_t positionX = 100,
		uint32_t positionY = 100
	) noexcept;

	void DestroyWindowInstance() const noexcept;
	
	LRESULT HandleMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

	std::shared_ptr<Keyboard> GetKeyboard() const noexcept;


private:
	/// Private Functions
	void SetWindowClass(std::shared_ptr<WindowAPIClass> pWindowClass) noexcept;
	std::shared_ptr<WindowAPIClass> GetWindowClass() const noexcept;

	void SetWidth(uint32_t width) noexcept;
	void SetHeight(uint32_t height) noexcept;
	void SetHandle(HWND handle) noexcept;
	void SetKeyboard(std::shared_ptr<Keyboard> pKeyboard) noexcept;

	/// Private Variables
	std::shared_ptr<WindowAPIClass> m_pWindowClass;

	HWND m_handle = NULL;
	uint32_t m_width = 0;
	uint32_t m_height = 0;

	std::shared_ptr<Keyboard> m_pKeyboard;


public:
	Window() = default;
	~Window() = default;

	// Copy sematics
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

	// Move semantics
	Window(Window&&) = delete;
	Window& operator = (Window&&) = delete;
};