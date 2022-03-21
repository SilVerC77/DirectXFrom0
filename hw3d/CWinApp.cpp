#include "CWinApp.h"
#include <sstream>
#include "resource.h"

#define APP_NAME L"First DX3D Window"

/***************************
*Window Class
****************************/
Window::WindowClass Window::WindowClass::wndClass;

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
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadimagea
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
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

/***************************
*Window
****************************/
Window::Window(int _width, int _height, const wchar_t* _name)
{
	//window size base on client region
	RECT wr;
	wr.left = 100;
	wr.right = _width + wr.left;
	wr.top = 100;
	wr.bottom = _height + wr.top;

	if (FAILED(AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE)))
	{
		throw MWND_LAST_EXCEPT();
	};

	//throw CHWND_EXCEPT(ERROR_ARENA_TRASHED);
	//throw std::runtime_error("error test");
	//throw 7908012-9;

	//create Window
	hWnd = CreateWindow(
		WindowClass::GetName(), _name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	// check for error
	if (hWnd == nullptr)
	{
		throw MWND_LAST_EXCEPT();
	}

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
	//https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-nccreate
	if (_msg == WM_NCCREATE) {
		//extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(_lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptra
		//set WinAPI-managed user data to store prt to window class
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
		//destructor destroy window so just return 0
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		//Keyboard==================================================
	case WM_KILLFOCUS:
		Keyboard.ClearState();
		break;

	case WM_KEYDOWN:
		//syskey
	case WM_SYSKEYDOWN:
		if (!(_lParam & 0x40000000 || Keyboard.IsAutorepeat())) {
			Keyboard.KeyPressed(static_cast<unsigned char>(_wParam));
			break;
		}

	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard.KeyReleased(static_cast<unsigned char>(_wParam));
		break;

	case WM_CHAR:
		Keyboard.OnChar(static_cast<unsigned char>(_wParam));
		break;
		//=================================================Keyboard
	}

	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}

/***************************
*Exception
****************************/
Window::Exception::Exception(int _line, const char* _file, HRESULT _hr)noexcept
	:CMyException(_line, _file)
	, hrResult(_hr)
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	sWhatBuffer = oss.str();
	return sWhatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "My Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT _hr) noexcept
{
	char* pmsgbuf = nullptr;
	//TODO:Without A
	DWORD nmsglen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER		//      テキストのメモリ割り当てを要求する
		| FORMAT_MESSAGE_FROM_SYSTEM		//      エラーメッセージはWindowsが用意しているものを使用
		| FORMAT_MESSAGE_IGNORE_INSERTS,	//      次の引数を無視してエラーコードに対するエラーメッセージを作成する
		nullptr, _hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&pmsgbuf, 0, nullptr
	);

	// 0 string length returned indicates a failure
	if (nmsglen == 0) {
		return "Unidentified error code";
	}
	//copy error string from window
	std::string errorString = pmsgbuf;
	//free window buffer
	LocalFree(pmsgbuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hrResult;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hrResult);
}
