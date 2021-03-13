#include "Application.h"

std::shared_ptr<Application> Application::CreateObject(std::shared_ptr<Window> pWindow) noexcept
{
	std::shared_ptr<Application> pApp = std::make_shared<Application>();
	
	if (!pWindow)
	{
		pWindow = Window::CreateObject();
		if (!pWindow) return nullptr;
		pWindow->Show();
	}
	pApp->SetWindow(pWindow);
	pApp->SetTimer(std::make_shared<Timer>());

	return pApp;
}
void Application::DestroyObject() noexcept
{
	m_pWindow->DestroyObject();
}


uint64_t Application::StartGameLoop() noexcept
{
	while (true)
	{
		if (const auto errorCode = Window::ProcessMessages())
		{
			return *errorCode;
		}

		if (!Frame())
		{
			break;
		}
	}

	return 0;
}

void Application::SetWindow(std::shared_ptr<Window> pWindow) noexcept
{
	m_pWindow = pWindow;
}
void Application::SetTimer(std::shared_ptr<Timer> pTimer) noexcept
{
	m_pTimer = pTimer;
}


bool Application::Frame() noexcept
{
	if (!m_pWindow->GetGraphics()->ClearBackBuffer(0.4f, 0.4f, 0.4f))
	{
		return false;
	}
	if (!m_pWindow->GetGraphics()->DrawTestCube(
		m_pTimer->Peek(), 
		2.0f * m_pWindow->GetMouse()->GetXPosition() / m_pWindow->GetWidth() - 1.0f, 
		-2.0f * m_pWindow->GetMouse()->GetYPosition() / m_pWindow->GetHeight() + 1.0f
	))
	{
		return false;
	}
	if (!m_pWindow->GetGraphics()->SwapFrames())
	{
		return false;
	}

	return true;
}