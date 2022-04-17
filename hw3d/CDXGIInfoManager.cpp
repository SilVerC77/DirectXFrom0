#include "CDXGIInfoManager.h"
#include "CWinApp.h"
#include "CGraphics.h"
#include <dxgidebug.h>
#include <memory>

#pragma comment(lib,"dxguid.lib")

#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw CGraphics::HrException( __LINE__,__FILE__,hr )

CDXGIInfoManager::CDXGIInfoManager()
{
	// define function signature of DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	// load the dll that contains the function DXGIGetDebugInterface
	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		throw MWND_LAST_EXCEPT();
	}

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);

	if (DxgiGetDebugInterface == nullptr) throw MWND_LAST_EXCEPT();

	HRESULT hr;
	GFX_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(&pDXGIInfoQueue)));
}

CDXGIInfoManager::~CDXGIInfoManager()
{
	if (pDXGIInfoQueue)	pDXGIInfoQueue->Release();
}

void CDXGIInfoManager::Set() noexcept
{
	// set the index (lNext) so that the next all to GetMessages()
	// will only get errors generated after this call
	//https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/dxgi-debug-id
	lNext = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> CDXGIInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = lNext; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;

		// get the size of message i in bytes
		//https://docs.microsoft.com/ja-jp/windows/win32/api/dxgidebug/nf-dxgidebug-idxgiinfoqueue-getmessage
		GFX_THROW_NOINFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);

		//https://docs.microsoft.com/en-us/windows/win32/api/dxgidebug/ns-dxgidebug-dxgi_info_queue_message
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message and push its description into the vector
		GFX_THROW_NOINFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}