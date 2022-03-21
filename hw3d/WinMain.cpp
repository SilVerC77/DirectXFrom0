#include "CWinApp.h"

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try {
		Window wnd(800, 300, L"A DX11 From 0");

		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (wnd.Keyboard.IsPressed(VK_MENU)) {
				MessageBox(nullptr, L"TODO", L"Alt is Pressed", MB_OK | MB_ICONEXCLAMATION);
			}
		}

		if (gResult == -1) {
			return -1;
		}

		return msg.wParam;
	}
	catch (const CMyException& e)
	{
		//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
		//TODO:make it success without A
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknow Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}