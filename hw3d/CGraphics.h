#pragma once
#include "ChiliWin.h"
#include "CMyException.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "CDXGIInfoManager.h"

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
		std::string sInfo;
	public:
		HrException(int _line, const char* _file, HRESULT _hr, std::vector<std::string>_info = {})noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo()const noexcept;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	private:
		std::string sReason;
	public:
		const char* GetType() const noexcept override;
	};
private:
#ifndef NDEBUG
	CDXGIInfoManager InfoManager;
#endif // !NDEBUG

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;

public:
	CGraphics(HWND _hWnd);
	CGraphics(const CGraphics&) = delete;
	CGraphics& operator=(const CGraphics&) = delete;
	~CGraphics() = default;

	void EndRender();
	void ClearBuffer(float _red, float _green, float _blue) noexcept;
};
