#include "CWinApp.h"

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	Window wnd(800, 300, L"GG");

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1) {
		return -1;
	}

	return msg.wParam;
}