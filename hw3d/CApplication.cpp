#include "CApplication.h"
#include <sstream>
#include <iomanip>

CApplication::CApplication()
	:Wnd(800, 600, L"DirectX YYDS")
{}

int CApplication::Run()
{
	long time = Timer.GetTime();
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
	const float dt = Timer.GetDeltaTime();
	std::ostringstream oss;
	oss << "Time elapsed: " << dt << "s";
	Wnd.SetTitle(oss.str());

}