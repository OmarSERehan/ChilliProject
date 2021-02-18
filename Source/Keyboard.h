#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	// Expose private members to Window class
	friend class Window;

public:
	class Event { 
	public: 
		enum class Type { Press, Release, Invalid }; 

		Event() : m_type(Type::Invalid), m_keyCode(0u) {}
		Event(Type type, unsigned char keyCode) : m_type(type), m_keyCode(keyCode) {}

		bool IsPressed() const noexcept { return m_type == Type::Press;  }
		bool IsReleased() const noexcept { return m_type == Type::Release;  }
		bool IsValid() const noexcept { return m_type != Type::Invalid; }

		unsigned GetKeyCode() const noexcept { return m_keyCode; }

	private:
		Type m_type;
		unsigned char m_keyCode;
	};

	// Keys
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKeyQueue() noexcept;

	// Characters
	char ReadChar() noexcept;
	bool CharIsEmpty() noexcept;
	void FlushCharQueue() noexcept;
	
	// Utilities
	void FlushAllQueues() noexcept;

	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool AutoRepeatIsEnabled() const noexcept;


private:
	/// Functions
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;

	void ClearKeysStates() noexcept;


	/// Variables & Containers
	static constexpr uint32_t s_numOfKeys = 256u;
	std::bitset<s_numOfKeys> m_keysStates;
	
	static constexpr uint32_t s_bufferMaxSize = 16u;
	std::queue<Event> m_keysBuffer;
	std::queue<char> m_charsBuffer;
	
	bool m_bAutoRepeatEnabled = false;


public:
	// Default constructor
	Keyboard() = default;

	// Copy semantics
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator = (const Keyboard&) = delete;

	// Move semantics
	Keyboard(Keyboard&&) = delete;
	Keyboard& operator = (Keyboard&&) = delete;
};

