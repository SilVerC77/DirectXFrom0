#include "CKeyboard.h"

bool CKeyboard::IsPressed(unsigned char _keycode) const
{
	return KeyState[_keycode];
}

CKeyboard::CEvent CKeyboard::ReadKey()
{
	if (KeyBuffer.size() > 0u) {
		CKeyboard::CEvent event = KeyBuffer.front();
		KeyBuffer.pop();
		return event;
	}
	else return CKeyboard::CEvent();
}

bool CKeyboard::IsEmptyKey() const
{
	return KeyBuffer.empty();
}

void CKeyboard::ClearKey()
{
	KeyBuffer = std::queue<CEvent>();
}

char CKeyboard::ReadChar()
{
	if (CharBuffer.size() > 0u) {
		unsigned char charcode = CharBuffer.front();
		CharBuffer.pop();
		return charcode;
	}
	else return 0;
}

bool CKeyboard::IsEmptyChar() const
{
	return CharBuffer.empty();
}

void CKeyboard::ClearChar()
{
	CharBuffer = std::queue<char>();
}

void CKeyboard::ClearAll()
{
	ClearChar();
	ClearKey();
}

void CKeyboard::SetAutorepear(bool _isRepeat)
{
	bAutoRepeat = _isRepeat;
}

void CKeyboard::EnableAutorepeat()
{
	bAutoRepeat = true;
}

void CKeyboard::DisableAutorepeat()
{
	bAutoRepeat = false;
}

bool CKeyboard::IsAutorepeat()
{
	return bAutoRepeat;
}

void CKeyboard::KeyPressed(unsigned char _keycode)
{
	KeyState[_keycode] = true;
	KeyBuffer.push(CKeyboard::CEvent(CKeyboard::CEvent::EType::Press, _keycode));
	TrimBuffer(KeyBuffer);
}

void CKeyboard::KeyReleased(unsigned char _keycode)
{
	KeyState[_keycode] = false;
	KeyBuffer.push(CKeyboard::CEvent(CKeyboard::CEvent::EType::Release, _keycode));
	TrimBuffer(KeyBuffer);
}

void CKeyboard::OnChar(char _char)
{
	CharBuffer.push(_char);
	TrimBuffer(CharBuffer);
}

void CKeyboard::ClearState()
{
	KeyState.reset();
}

template<typename T>
void CKeyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > iBufferSize) {
		buffer.pop();
	}
}

