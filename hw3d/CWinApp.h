/*
*
*
*/
#pragma once
#include "ChiliWin.h"
#include "CMyException.h"

class Window
{
public:
	class Exception :public CMyException
	{
		Exception(int _line, const char* file, HRESULT _hr)noexcept;
		const char* what()const noexcept override;
		virtual const char* GetType()const noexcept override;
		static std::string TranslateErrorCode(HRESULT _hr)noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString()const noexcept;
	private:
		HRESULT hrResult;
	};
private:
	//Singleton
	class WindowClass
	{
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

private:
	int iWidth;
	int iHeight;
	HWND hWnd;

public:
	Window(int _width, int _height, const wchar_t* _name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept;
	LRESULT HandleMsg(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept;
};

//error exception helper macro
#define CHWND_EXCEPT(hr) Window::Exception(__LINE__,__FILE__,hr)