#include "CTimer.h"

CTimer::CTimer()
	:fDeltaTime(0.f)
{
	Mark();
}

void CTimer::UpdateTimer()
{
	using namespace std::chrono;

	const auto previous = NowTime;
	NowTime = steady_clock::now();
	const duration<float, std::milli> frametime = NowTime - previous;
	fDeltaTime = frametime.count();
}

float CTimer::GetDeltaTime() const
{
	return fDeltaTime;
}

float CTimer::GetTime()const
{
	const auto now = std::chrono::steady_clock::now();
	const auto time = now - MarkTime;
	return std::chrono::duration<float>(time).count();	
}

void CTimer::Mark()
{
	MarkTime = std::chrono::steady_clock::now();
}