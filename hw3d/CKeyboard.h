#pragma once
#include <queue>
#include <bitset>

class CKeyboard
{
	friend class Window;
public:
	class CEvent
	{
	public:
		enum class EType
		{
			Press,
			Release,
			Unknown
		};
	private:
		EType Type;
		unsigned char cCode;
	public:
		CEvent()
			:Type(EType::Unknown)
			, cCode(0u)
		{}
		CEvent(EType _type, unsigned char _code)
			:Type(_type)
			, cCode(_code)
		{}
		bool IsPress()const { return Type == EType::Press; }
		bool IsRelease()const { return Type == EType::Release; }
		bool IsValid()const { return Type != EType::Unknown; }
		unsigned char GetKeyCode() { return cCode; }
	};
private:
	static constexpr unsigned int iKey = 256u;
	static constexpr unsigned int iBufferSize = 16u;
	bool bAutoRepeat = false;
	std::bitset<iKey>KeyState;
	std::queue<CEvent>KeyBuffer;
	std::queue<char>CharBuffer;

public:
	CKeyboard() = default;

	//key event
	//https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
	bool IsPressed(unsigned char _keycode)const;
	CEvent ReadKey();
	bool IsEmptyKey()const;
	void ClearKey();

	//char event
	char ReadChar();
	bool IsEmptyChar()const;
	void ClearChar();

	void ClearAll();

	//autorepeat
	void SetAutorepear(bool _isRepeat);
	void EnableAutorepeat();
	void DisableAutorepeat();
	bool IsAutorepeat();

private:
	void KeyPressed(unsigned char _keycode);
	void KeyReleased(unsigned char _keycode);
	void OnChar(char _char);
	void ClearState();
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
};
