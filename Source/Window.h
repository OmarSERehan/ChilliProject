#pragma once
#include <utility>
#include <iostream>
#include <optional>
#include <functional>

#include "ErrorHandler.h"
#include "WindowAPIClass.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"


class Window
{
public:
	static std::shared_ptr<Window> CreateObject(
		LPCTSTR windowTitle = L"Window Title",
		int32_t clientWidth = 800,
		int32_t clientHeight = 600,
		int32_t positionX = 100,
		int32_t positionY = 100,
		std::shared_ptr<WindowAPIClass> pWindowClass = nullptr,
		std::shared_ptr<Graphics> pGraphics = nullptr) noexcept;
	bool DestroyObject() noexcept;

	bool Show() const noexcept;
	bool Hide() const noexcept;
	
	static std::optional<uint64_t> ProcessMessages() noexcept;
	LRESULT HandleMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

	bool SetTitle(const std::wstring& title) noexcept;

	std::shared_ptr<WindowAPIClass> GetWindowClass() const noexcept;
	std::shared_ptr<Keyboard> GetKeyboard() const noexcept;
	std::shared_ptr<Mouse> GetMouse() const noexcept;
	std::shared_ptr<Graphics> GetGraphics() const noexcept;
	int32_t GetWidth() const noexcept;
	int32_t GetHeight() const noexcept;


private:
	void SetWindowClass(std::shared_ptr<WindowAPIClass> pWindowClass) noexcept;
	void SetWidth(int32_t width) noexcept;
	void SetHeight(int32_t height) noexcept;
	void SetHandle(HWND handle) noexcept;
	void SetKeyboard(std::shared_ptr<Keyboard> pKeyboard) noexcept;
	void SetMouse(std::shared_ptr<Mouse> pMouse) noexcept;
	void SetGraphics(std::shared_ptr<Graphics> pGraphics) noexcept;


	HWND m_handle = NULL;
	int32_t m_width = 0;
	int32_t m_height = 0;
	LPCTSTR m_title = L"";

	std::shared_ptr<WindowAPIClass> m_pWindowClass;
	std::shared_ptr<Keyboard> m_pKeyboard;
	std::shared_ptr<Mouse> m_pMouse;
	std::shared_ptr<Graphics> m_pGraphics;


public:
	Window() = default;
	~Window() = default;

	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

	Window(Window&&) = delete;
	Window& operator = (Window&&) = delete;
};