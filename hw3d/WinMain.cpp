#include <Windows.h>
#include <tchar.h>
#include <string>
#include <sstream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(69);
		break;

	case WM_KEYDOWN:
		if (wParam == 'F') {
			SetWindowText(hWnd, _T("GG.COM"));
		}
		break;

	case WM_KEYUP:
		if (wParam == 'F') {
			SetWindowText(hWnd, _T("YYDS"));
		}
		break;

	case WM_CHAR: {
		static std::wstring title;
		title.push_back((char)wParam);
		SetWindowText(hWnd, title.c_str());
		break;
	}
	case WM_LBUTTONDOWN: {
		POINTS pt = MAKEPOINTS(lParam);
		std::ostringstream oss;
		oss << "(" << pt.x << "," << pt.y << ")";

		SetWindowTextA(hWnd, oss.str().c_str());
		break;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//register windwo class
	const auto pClassName = _T("first DirectX");
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

	//create window instance
	HWND hWnd = CreateWindowEx(
		0, pClassName,
		_T("Happy DirectX"),
		WS_OVERLAPPEDWINDOW,
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);

	//message
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1) {
		return -1;
	}
	else {
		return msg.wParam;
	}

	return 0;
}