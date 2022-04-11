#pragma once
#include <chrono>

class CTimer
{
	std::chrono::steady_clock::time_point NowTime;
	std::chrono::steady_clock::time_point MarkTime;
	float fDeltaTime;
public:
	CTimer();
	void UpdateTimer();
	float GetDeltaTime()const;
	float GetTime() const;
	void Mark();
};