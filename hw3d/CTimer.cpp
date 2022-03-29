#include "CTimer.h"

CTimer::CTimer()
	:fDeltaTime(0.f)
{}

void CTimer::UpdateTimer()
{
	using namespace std::chrono;

	const auto previous = nowTime;
	nowTime = steady_clock::now();
	const duration<float> frametime = nowTime - previous;
	fDeltaTime = frametime.count();
}

float CTimer::GetDeltaTime() const
{
	return fDeltaTime;
}

long CTimer::GetTime()const
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
