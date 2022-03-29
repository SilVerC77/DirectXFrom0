#pragma once
#include "CWinApp.h"
#include "CTimer.h"

class CApplication 
{
private:
	Window Wnd;
	CTimer Timer;
public:
	CApplication();
	int Run();

private:
	void Loop();
};