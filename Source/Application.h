#pragma once
#include "Window.h"
#include "Timer.h"
#include <iomanip>
#include "Cube.h"

class Application
{
public:
	static std::shared_ptr<Application> CreateObject(std::shared_ptr<Window> pWindow = nullptr) noexcept;
	void DestroyObject() noexcept;

	uint64_t StartGameLoop() noexcept;

	void SetWindow(std::shared_ptr<Window> pWindow) noexcept;
	void SetTimer(std::shared_ptr<Timer> pTimer) noexcept;

private:
	bool Frame() noexcept;

	std::shared_ptr<Window> m_pWindow;
	std::shared_ptr<Timer> m_pTimer;

	std::vector<std::unique_ptr<Cube>> m_boxes;

	void AddBox(std::unique_ptr<Cube> pNewBox) noexcept;

	std::shared_ptr<Window> GetWindow() const noexcept;

public:
	Application() = default;
};

