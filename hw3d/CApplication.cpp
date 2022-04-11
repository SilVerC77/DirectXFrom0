#include "CApplication.h"
#include <sstream>
#include <iomanip>

CApplication::CApplication()
	:Wnd(800, 600, L"DirectX YYDS")
{}

int CApplication::Run()
{
	float time = Timer.GetTime();
	while (true) {
		//process pending message
		if (const auto message = Window::ProcessMessages()) {
			return *message;
		}

		Loop();

		Sleep(1);  //!!Without this,pc will become lag.TODO:Fix it without Sleep;

	}

}

void CApplication::Loop()
{
	Timer.UpdateTimer();
	const float dt = Timer.GetTime();
	std::ostringstream oss;
	oss << "Time elapsed: " << dt << "s";
	Wnd.SetTitle(oss.str());

	const float c = sin(Timer.GetTime() / 2.f + .5f);
	Wnd.GetGraphics().ClearBuffer(c, c, 1.f);
	Wnd.GetGraphics().EndRender();

}