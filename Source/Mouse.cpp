#include "Mouse.h"
#include "WinHeader.h"

int32_t Mouse::GetXPosition() const noexcept
{
	return m_xPosition;
}
int32_t Mouse::GetYPosition() const noexcept
{
	return m_yPosition;
}

bool Mouse::IsLeftPressed() const noexcept
{
	return m_bLeftPressed;
}
bool Mouse::IsRightPressed() const noexcept
{
	return m_bRightPressed;
}
bool Mouse::IsInWindow() const noexcept
{
	return m_bInWindow;
}

Mouse::Event Mouse::Read() noexcept
{
	if (m_buffer.size() > 0u)
	{
		Mouse::Event e = m_buffer.front();
		m_buffer.pop();
		return e;
	}

	return Event();
}
bool Mouse::IsEmpty() const noexcept
{
	return m_buffer.empty();
}
void Mouse::Flush() noexcept
{
	m_buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int32_t x, int32_t y) noexcept
{
	m_xPosition = x;
	m_yPosition = y;

	m_buffer.push(Event(Event::Type::Move, *this));
	if (m_buffer.size() > s_bufferSize) m_buffer.pop();
}
void Mouse::OnMouseEnter() noexcept
{
	m_bInWindow = true;

	m_buffer.push(Event(Event::Type::Enter, *this));
	if (m_buffer.size() > s_bufferSize) m_buffer.pop();
}
void Mouse::OnMouseLeave() noexcept
{
	m_bInWindow = false;

	m_buffer.push(Event(Event::Type::Leave, *this));
	if (m_buffer.size() > s_bufferSize) m_buffer.pop();
}

void Mouse::OnLeftPressed(int32_t x, int32_t y) noexcept
{
	m_bLeftPressed = true;

	m_buffer.push(Event(Event::Type::LPress, *this));
	if (m_buffer.size() > s_bufferSize) m_buffer.pop();
}
void Mouse::OnLeftReleased(int32_t x, int32_t y) noexcept
{
	m_bLeftPressed = false;

	m_buffer.push(Event(Event::Type::LRelease, *this));
	if (m_buffer.size() > s_bufferSize) m_buffer.pop();
}

void Mouse::OnRightPressed(int32_t x, int32_t y) noexcept
{
	m_bRightPressed = true;

	m_buffer.push(Event(Event::Type::RPress, *this));
	if (m_buffer.size() > s_bufferSize) m_buffer.pop();
}
void Mouse::OnRightReleased(int32_t x, int32_t y) noexcept
{
	m_bRightPressed = false;

	m_buffer.push(Event(Event::Type::RRelease, *this));
	if (m_buffer.size() > s_bufferSize) m_buffer.pop();
}

void Mouse::OnWheelUp(int32_t x, int32_t y) noexcept
{
	m_buffer.push(Event(Event::Type::WheelUp, *this));
	if (m_buffer.size() > s_bufferSize) m_buffer.pop();
}
void Mouse::OnWheelDown(int32_t x, int32_t y) noexcept
{
	m_buffer.push(Event(Event::Type::WheelDown, *this));
	if (m_buffer.size() > s_bufferSize) m_buffer.pop();
}
void Mouse::OnWheelDelta(int32_t x, int32_t y, int32_t delta) noexcept
{
	m_wheelDeltaCarry += delta;

	while (m_wheelDeltaCarry >= WHEEL_DELTA)
	{
		m_wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (m_wheelDeltaCarry <= -WHEEL_DELTA)
	{
		m_wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}