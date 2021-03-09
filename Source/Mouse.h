#pragma once

#include <queue>

class Mouse
{
	friend class Window;

public:
	class Event {
	public:
		enum class Type { LPress, LRelease, RPress, RRelease, WheelUp, WheelDown, Move, Enter, Leave, Invalid };

		Event() noexcept :
			m_type(Type::Invalid),
			m_bLeftPressed(false),
			m_bRightPressed(false),
			m_xPosition(0),
			m_yPosition(0)
		{}

		Event(Type type, const Mouse& parent) noexcept :
			m_type(type),
			m_bLeftPressed(parent.IsLeftPressed()),
			m_bRightPressed(parent.IsRightPressed()),
			m_xPosition(parent.GetXPosition()),
			m_yPosition(parent.GetYPosition())
		{}

		Type GetType() const noexcept { return m_type; }
		int32_t GetXPosition() const noexcept { return m_xPosition; }
		int32_t GetYPosition() const noexcept { return m_yPosition; }

		bool IsValid() const noexcept { return m_type != Type::Invalid; }
		bool IsLeftPressed() const noexcept { return m_bLeftPressed; }
		bool IsRightPressed() const noexcept { return m_bRightPressed; }


	private:
		Type m_type;

		bool m_bLeftPressed;
		bool m_bRightPressed;

		int32_t m_xPosition;
		int32_t m_yPosition;
	};

	int32_t GetXPosition() const noexcept;
	int32_t GetYPosition() const noexcept;

	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	bool IsInWindow() const noexcept;

	Event Read() noexcept;
	bool IsEmpty() const noexcept;
	void Flush() noexcept;

private:
	/// Window Functions
	void OnMouseMove(int32_t x, int32_t y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;
	
	void OnLeftPressed(int32_t x, int32_t y) noexcept;
	void OnLeftReleased(int32_t x, int32_t y) noexcept;

	void OnRightPressed(int32_t x, int32_t y) noexcept;
	void OnRightReleased(int32_t x, int32_t y) noexcept;

	void OnWheelUp(int32_t x, int32_t y) noexcept;
	void OnWheelDown(int32_t x, int32_t y) noexcept;
	void OnWheelDelta(int32_t x, int32_t y, int32_t delta) noexcept;


	/// Variables & Containers
	static constexpr uint32_t s_bufferSize = 16u;
	std::queue<Event> m_buffer;

	int32_t m_xPosition = 0;
	int32_t m_yPosition = 0;
	int32_t m_wheelDeltaCarry = 0;

	bool m_bLeftPressed = false;
	bool m_bRightPressed = false;
	bool m_bInWindow = false;


public:
	Mouse() = default;

	Mouse(const Mouse&) = delete;
	Mouse& operator = (const Mouse&) = delete;

	Mouse(const Mouse&&) = delete;
	Mouse& operator = (const Mouse&&) = delete;
};