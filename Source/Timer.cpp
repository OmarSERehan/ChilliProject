#include "Timer.h"

Timer::Timer()
{
	m_lastTimePoint = std::chrono::steady_clock::now();
}


float Timer::Mark() noexcept
{
	const auto currentTimePoint = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = currentTimePoint - m_lastTimePoint;
	m_lastTimePoint = currentTimePoint;
	return frameTime.count();
}

float Timer::Peek() const noexcept
{
	const auto currentTimePoint = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = currentTimePoint - m_lastTimePoint;
	return frameTime.count();
}