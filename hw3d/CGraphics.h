#pragma once
#include "ChiliWin.h"
#include "CMyException.h"
#include <d3d11.h>

class CGraphics
{
public:

	/// Exceprion Handle
	class Exception : public CMyException
	{
		using CMyException::CMyException;
	};
	class HrException :public Exception
	{
		HRESULT hr;
	public:
		HrException(int _line, const char* _file, HRESULT _hr)noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	};

	CGraphics(HWND _hWnd);
	CGraphics(const CGraphics&) = delete;
	CGraphics& operator=(const CGraphics&) = delete;
	~CGraphics();

	void EndRender();
	void ClearBuffer(float _red, float _green, float _blue) noexcept;

private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};