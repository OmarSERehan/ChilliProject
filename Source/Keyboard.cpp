#include "Keyboard.h"

// Keys
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return m_keysStates[keycode];
}
Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (!m_keysBuffer.empty())
	{
		Keyboard::Event e = m_keysBuffer.front();
		m_keysBuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}
bool Keyboard::KeyIsEmpty() const noexcept
{
	return m_keysBuffer.empty();
}
void Keyboard::FlushKeyQueue() noexcept
{
	m_keysBuffer = std::queue<Event>();
}

// Characters
char Keyboard::ReadChar() noexcept
{
	if (!m_charsBuffer.empty())
	{
		unsigned char characterCode = m_charsBuffer.front();
		m_charsBuffer.pop();
		return characterCode;
	}
	else
	{
		return 0;
	}
}
bool Keyboard::CharIsEmpty() noexcept
{
	return m_charsBuffer.empty();
}
void Keyboard::FlushCharQueue() noexcept
{
	m_charsBuffer = std::queue<char>();
}

// Utilities
void Keyboard::FlushAllQueues() noexcept
{
	FlushKeyQueue();
	FlushCharQueue();
}

void Keyboard::EnableAutoRepeat() noexcept
{
	m_bAutoRepeatEnabled = true;
}
void Keyboard::DisableAutoRepeat() noexcept
{
	m_bAutoRepeatEnabled = false;
}
bool Keyboard::AutoRepeatIsEnabled() const noexcept
{
	return m_bAutoRepeatEnabled;
}


/// Private Functions for Window class
void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	m_keysStates[keycode] = true;
	m_keysBuffer.push(Keyboard::Event(Event::Type::Press, keycode));
	
	if (m_keysBuffer.size() > s_bufferMaxSize) m_keysBuffer.pop();
}
void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	m_keysStates[keycode] = false;
	m_keysBuffer.push(Keyboard::Event(Event::Type::Release, keycode));
	if (m_keysBuffer.size() > s_bufferMaxSize) m_keysBuffer.pop();
}
void Keyboard::OnChar(char character) noexcept
{
	m_charsBuffer.push(character);
	if (m_charsBuffer.size() > s_bufferMaxSize) m_charsBuffer.pop();
}

void Keyboard::ClearKeysStates() noexcept
{
	m_keysStates.reset();
}