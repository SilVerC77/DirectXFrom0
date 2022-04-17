#include "CGraphics.h"
#include "dxerr.h"
#include <sstream>

#pragma comment(lib,"d3d11.lib")

//Debug
// graphics exception checking/throwing macros (some with dxgi info)
#define GFX_EXCEPT_NOINFO(hr) CGraphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw CGraphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GFX_EXCEPT(hr) CGraphics::HrException( __LINE__,__FILE__,(hr),InfoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) InfoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) CGraphics::DeviceRemovedException( __LINE__,__FILE__,(hr),InfoManager.GetMessages() )
#else
#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif

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

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//check results of d3d functions
	HRESULT hr;


	//https://docs.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3ddevice9-createadditionalswapchain 
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-clearrendertargetview
	//access texture Subresource in swap chain(back buffer)
	Microsoft::WRL::ComPtr<ID3D11Resource> backbuffer = nullptr;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &backbuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(backbuffer.Get(), nullptr, &pTarget));
}

void CGraphics::EndRender()
{
	HRESULT hr;

#ifndef NDEBUG
	InfoManager.Set();
#endif // !NDEBUG

	if (FAILED(hr = pSwap->Present(1u, 0u))) {
		//handle error etc: suddenly remove graphic card,driver crash
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			//https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11device-getdeviceremovedreason
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else {
			throw GFX_EXCEPT(hr);
		}
	}
}

void CGraphics::ClearBuffer(float _red, float _green, float _blue) noexcept
{
	const float color[] = { _red,_green,_blue,1.f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

// Graphics exception stuff
CGraphics::HrException::HrException(int _line, const char* _file, HRESULT _hr, std::vector<std::string>_info) noexcept
	: Exception(_line, _file)
	, hr(_hr)
{
	//join all messages with newlines into single string
	for (const auto& m : _info) {
		sInfo += m;
		sInfo.push_back('\n');
	}
	//remove final newline if exist
	if (!_info.empty()) {
		_info.pop_back();
	}
}

const char* CGraphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!sInfo.empty())
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	sWhatBuffer = oss.str();
	return sWhatBuffer.c_str();
}

const char* CGraphics::HrException::GetType() const noexcept
{
	return "My Graphics Exception";
}

HRESULT CGraphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string CGraphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorStringA(hr);
}

std::string CGraphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescriptionA(hr, buf, sizeof(buf));
	return buf;
}

std::string CGraphics::HrException::GetErrorInfo() const noexcept
{
	return sInfo;
}


const char* CGraphics::DeviceRemovedException::GetType() const noexcept
{
	return "MyGraphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}