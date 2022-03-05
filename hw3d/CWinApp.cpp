#include "CWinApp.h"

#define APP_NAME L"First DX3D Window"

//Window Class
Window::WindowClass Window::WindowClass::wndClass;

//constexpr const wchar_t* Window::WindowClass::wndClassName = APP_NAME;

Window::WindowClass::WindowClass() noexcept
	:hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance()noexcept
{
	return wndClass.hInst;
}

//Window
Window::Window(int _width, int _height, const wchar_t* _name) noexcept
{
	//window size base on client region
	RECT wr;
	wr.left = 100;
	wr.right = _width + wr.left;
	wr.top = 100;
	wr.bottom = _height + wr.top;
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	//create Window
	hWnd = CreateWindow(
		WindowClass::GetName(), _name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	//show Window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	//Only destroy window from destructor
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept
{
	//use create param passed CreateWindow() to store window class pointer
	if (_msg == WM_NCCREATE) {
		//extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(_lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//set WinAPI-managed user data ti store prt to window class
		SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//set message proc to normal handler ,setup finished
		SetWindowLongPtr(_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return pWnd->HandleMsg(_hWnd, _msg, _wParam, _lParam);
	}

	//if we get a message before the WM_NCCREATE message,handle with default handle
	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept
{
	//retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));

	return pWnd->HandleMsg(_hWnd, _msg, _wParam, _lParam);
}

LRESULT Window::HandleMsg(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) noexcept
{
	switch (_msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}
