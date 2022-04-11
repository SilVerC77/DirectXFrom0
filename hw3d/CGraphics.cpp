#include "CGraphics.h"
#pragma comment(lib,"d3d11.lib")

CGraphics::CGraphics(HWND _hWnd)
{
	//https://docs.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = _hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	//https://docs.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3ddevice9-createadditionalswapchain 
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-clearrendertargetview
	//access texture Subresource in swap chain(back buffer)
	ID3D11Resource* backbuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backbuffer));
	pDevice->CreateRenderTargetView(backbuffer, nullptr, &pTarget);
	backbuffer->Release();
}

CGraphics::~CGraphics()
{
	if (pTarget) pTarget->Release();
	if (pContext) pContext->Release();
	if (pSwap) pDevice->Release();
	if (pDevice) pDevice->Release();
}

void CGraphics::EndRender()
{
	pSwap->Present(1u, 0u);
}

void CGraphics::ClearBuffer(float _red, float _green, float _blue) noexcept
{
	const float color[] = { _red,_green,_blue,1.f };
	pContext->ClearRenderTargetView(pTarget, color);
}
