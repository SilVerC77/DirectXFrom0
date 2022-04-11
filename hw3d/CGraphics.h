#pragma once
#include "ChiliWin.h"
#include <d3d11.h>

class CGraphics
{
public:
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
