/*
*
*
*/
#pragma once
#include "ChiliWin.h"
#include "CMyException.h"
#include "CKeyboard.h"
#include "CMouse.h"
#include "CGraphics.h"
#include <optional>
#include <memory>

class Window
{
public:
	class Exception :public CMyException
	{
		using CMyException::CMyException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};

	class HrException:public Exception
	{
	private:
		HRESULT hrResult;
	public:
		HrException(int _line, const char* _file, HRESULT _hr)noexcept;
		const char* what()const noexcept override;
		const char* GetType()const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription()const noexcept;
	};

	class NoGraphicsException :public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType()const noexcept override;
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

public:
	CKeyboard Keyboard;
	CMouse Mouse;

private:
	int iWidth;
	int iHeight;
	HWND hWnd;
	std::unique_ptr<CGraphics> pGraphics;

public:
	Window(int _width, int _height, const wchar_t* _name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& _title);
	static std::optional<int> ProcessMessages();

	CGraphics& GetGraphics();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept;
	LRESULT HandleMsg(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)noexcept;
};

//error exception helper macro
#define MWND_EXCEPT(hr) Window::HrException(__LINE__,__FILE__,hr)
#define MWND_LAST_EXCEPT() Window::HrException(__LINE__,__FILE__,GetLastError())
#define MWND_NOGFX_EXCEPT() Window::NoGraphicsException( __LINE__,__FILE__ ) 