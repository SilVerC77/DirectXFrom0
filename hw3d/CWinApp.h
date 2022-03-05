/*
*
*
*/
#pragma once
#include "ChiliWin.h"

class Window {
private:
	//Singleton
	class WindowClass {
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* wndClassName = L"DX11";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int _width, int _height, const wchar_t* _name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept;
	LRESULT HandleMsg(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept;

private:
	int iWidth;
	int iHeight;
	HWND hWnd;
};