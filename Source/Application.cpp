#include "Application.h"


Application::Application()
	:
	m_pWindow(std::make_shared<Window>()),
	m_pTimer(std::make_shared<Timer>())
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> rDistribution(6.0f, 20.0f);
	std::uniform_real_distribution<float> grDistribution(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> laDistribution(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> gaDistribution(0.0f, 3.1415f * 2.0f);
	for (uint16_t i = 0u; i < 30u; i++)
	{
		m_pBoxes.push_back(std::make_unique<Box>(m_pWindow->GetGraphics(), rng, rDistribution, grDistribution, laDistribution, gaDistribution));
	}

	//m_pTriangle = std::make_unique<Triangle>(m_pWindow->GetGraphics()());

	m_pWindow->GetGraphics()->SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 0.75f, 0.5f, 40.0f));
}


uint64_t Application::StartGameLoop() noexcept
{
	m_pWindow->Show();

	while (true)
	{
		if (const auto errorCode = Window::ProcessMessages())
		{
			return *errorCode;
		}

		Frame();
	}

	return 0;
}

void Application::Frame()
{
	float deltaTime = m_pTimer->Mark();
	m_pWindow->GetGraphics()->ClearBackBuffer(0.4f, 0.4f, 0.4f);
	
	//m_pWindow->GetGraphics()->DrawTestTriangle(5.0f, 0.0f, 0.0f);
	//m_pTriangle->Draw(m_pWindow->GetGraphics()());
	
	for (auto& pBox : m_pBoxes)
	{
		pBox->Update(deltaTime);
		pBox->Draw(m_pWindow->GetGraphics());
	}

	m_pWindow->GetGraphics()->SwapFrames();
}