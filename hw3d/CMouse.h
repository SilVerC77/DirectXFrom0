#pragma once
#include <queue>

class CMouse {
	friend class Window;

public:
	class CEvent {
	public:
		enum class EType {
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Unknow
		};

	private:
		EType Type;
		bool bLeftPressed;
		bool bRightPressed;
		int iX;
		int iY;

	public:
		CEvent()
			:Type(EType::Unknow)
			, bLeftPressed(false)
			, bRightPressed(false)
			, iX(0)
			, iY(0)
		{}
		CEvent(EType _type, const CMouse& _parent)
			:Type(_type)
			, bLeftPressed(_parent.bLeftPressed)
			, bRightPressed(_parent.bRightPressed)
			, iX(_parent.iX)
			, iY(_parent.iY)
		{}

		bool IsValid()const { return Type != EType::Unknow; }
		EType GetType()const { return Type; }
		std::pair<int, int>GetPos()const { return { iX,iY }; }
		int GetPosX()const { return iX; }
		int GetPosY()const { return iY; }
		bool IsLeftPressed()const { return bLeftPressed; }
		bool IsRightPressed()const { return bRightPressed; }
	};

private:
	static constexpr unsigned int iBufferSize = 16u;
	int iX;
	int iY;
	int iWheelDeltaCarry = 0;
	bool bLeftPressed = false;
	bool bRightPressed = false;
	bool bInWindow = false;
	std::queue<CEvent> Buffer;

public:
	CMouse() = default;
	std::pair<int, int>GetPose()const;
	int GetPosX()const;
	int GetPosY()const;
	bool IsLeftPressed()const;
	bool IsRightPressed()const;
	bool IsInWindow()const;
	CMouse::CEvent Read();
	bool IsEmpty()const { return Buffer.empty(); }
	void Clear();

private:
	void MouseMove(int _x, int _y);
	void MouseEnter();
	void MouseLeave();
	void LeftPressed();
	void LeftReleased();
	void RightPressed();
	void RightReleased();
	void WheelUp();
	void WheelDown();
	void WheelDelta(int _delta);
	void TrimBuffer();
};
