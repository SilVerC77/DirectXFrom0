#include "CApplication.h"

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try {
		return CApplication{}.Run();
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
