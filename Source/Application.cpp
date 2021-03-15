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


	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> rDistribution(6.0f, 20.0f);
	std::uniform_real_distribution<float> grDistribution(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> laDistribution(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> gaDistribution(0.0f, 3.1415f * 2.0f);
	for (uint16_t i = 0u; i < 10; i++)
	{
		pApp->AddBox(Cube::CreateObject(pApp->GetWindow()->GetGraphics().get(), rng, rDistribution, grDistribution, laDistribution, gaDistribution));
	}
	pApp->GetWindow()->GetGraphics()->SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 0.75f, 0.1f, 10.0f));

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


std::shared_ptr<Window> Application::GetWindow() const noexcept
{
	return m_pWindow;
}

bool Application::Frame() noexcept
{
	float deltaTime = m_pTimer->Mark();
	if (!m_pWindow->GetGraphics()->ClearBackBuffer(0.4f, 0.4f, 0.4f))
	{
		return false;
	}
	
	//m_pWindow->GetGraphics()->DrawTestTriangle(0.0f, 0.0f, 0.0f);

	for (auto& pBox : m_boxes)
	{
		pBox->Update(deltaTime);
		pBox->Draw(m_pWindow->GetGraphics().get());
	}

	if (!m_pWindow->GetGraphics()->SwapFrames())
	{
		return false;
	}

	return true;
}

void Application::AddBox(std::unique_ptr<Cube> pNewBox) noexcept
{
	m_boxes.push_back(std::move(pNewBox));
}