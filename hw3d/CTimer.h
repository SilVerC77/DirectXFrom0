#pragma once
#include <chrono>

class CTimer
{
	std::chrono::steady_clock::time_point nowTime;
	float fDeltaTime;
public:
	CTimer();
	void UpdateTimer();
	float GetDeltaTime()const;
	long GetTime() const;
};