#include "CMouse.h"
#include <Windows.h>

std::pair<int, int> CMouse::GetPose() const
{
	return { iX,iY };
}

int CMouse::GetPosX() const
{
	return iX;
}

int CMouse::GetPosY() const
{
	return iY;
}

bool CMouse::IsLeftPressed() const
{
	return bLeftPressed;
}

bool CMouse::IsRightPressed() const
{
	return bRightPressed;
}

bool CMouse::IsInWindow() const
{
	return bInWindow;
}

CMouse::CEvent CMouse::Read()
{
	if (Buffer.size() > 0u) {
		CMouse::CEvent event = Buffer.front();
		Buffer.pop();
		return event;
	}
	else {
		return CMouse::CEvent();
	}
}

void CMouse::Clear()
{
	Buffer = std::queue<CEvent>();
}

void CMouse::MouseMove(int _x, int _y)
{
	iX = _x;
	iY = _y;

	Buffer.push(CMouse::CEvent(CMouse::CEvent::EType::Move, *this));
	TrimBuffer();
}

void CMouse::MouseEnter()
{
	bInWindow = true;
	Buffer.push(CMouse::CEvent(CMouse::CEvent::EType::Enter, *this));
	TrimBuffer();
}

void CMouse::MouseLeave()
{
	bInWindow = false;
	Buffer.push(CMouse::CEvent(CMouse::CEvent::EType::Leave, *this));
	TrimBuffer();
}

void CMouse::LeftPressed()
{
	bLeftPressed = true;
	Buffer.push(CMouse::CEvent(CMouse::CEvent::EType::LPress, *this));
	TrimBuffer();
}

void CMouse::LeftReleased()
{
	bLeftPressed = false;
	Buffer.push(CMouse::CEvent(CMouse::CEvent::EType::LRelease, *this));
	TrimBuffer();
}

void CMouse::RightPressed()
{
	bRightPressed = true;
	Buffer.push(CMouse::CEvent(CMouse::CEvent::EType::RPress, *this));
	TrimBuffer();
}

void CMouse::RightReleased()
{
	bLeftPressed = false;
	Buffer.push(CMouse::CEvent(CMouse::CEvent::EType::RRelease, *this));
	TrimBuffer();
}

void CMouse::WheelUp()
{
	Buffer.push(CMouse::CEvent(CMouse::CEvent::EType::WheelUp, *this));
	TrimBuffer();
}

void CMouse::WheelDown()
{
	Buffer.push(CMouse::CEvent(CMouse::CEvent::EType::WheelUp, *this));
	TrimBuffer();
}

void CMouse::WheelDelta(int _delta)
{
	iWheelDeltaCarry += _delta;
	//event every 120(WHEEL_DELTA)
	while (iWheelDeltaCarry >= WHEEL_DELTA) {
		iWheelDeltaCarry -= WHEEL_DELTA;
		WheelUp();
	}
	while (iWheelDeltaCarry <= -WHEEL_DELTA) {
		iWheelDeltaCarry += WHEEL_DELTA;
		WheelDown();
	}
}

void CMouse::TrimBuffer()
{
	while (Buffer.size() > iBufferSize) {
		Buffer.pop();
	}
}
