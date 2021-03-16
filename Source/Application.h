#pragma once
#include "Window.h"
#include "Timer.h"
#include <iomanip>
#include "Cube.h"
#include "Triangle.h"

class Application
{
public:
	Application();

	uint64_t StartGameLoop() noexcept;

private:
	void Frame();

	std::shared_ptr<Window> m_pWindow = nullptr;
	std::shared_ptr<Timer> m_pTimer = nullptr;

	std::vector<std::unique_ptr<Cube>> m_pBoxes;
	std::unique_ptr<Triangle> m_pTriangle = nullptr;
};