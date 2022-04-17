#pragma once
#include "ChiliWin.h"
#include <wrl.h>
#include <vector>
#include <string>
#include <dxgidebug.h>

class CDXGIInfoManager
{
public:
	CDXGIInfoManager();
	~CDXGIInfoManager()=default;
	CDXGIInfoManager(const CDXGIInfoManager&) = delete;
	CDXGIInfoManager& operator=(const CDXGIInfoManager&) = delete;
	
	void Set()noexcept;
	std::vector<std::string> GetMessages()const;

private:
	unsigned long long lNext = 0u;
	Microsoft::WRL::ComPtr<struct IDXGIInfoQueue> pDXGIInfoQueue = nullptr;
};