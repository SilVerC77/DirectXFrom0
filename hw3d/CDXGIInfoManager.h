#pragma once
#include "ChiliWin.h"
#include <vector>
#include <string>

class CDXGIInfoManager
{
public:
	CDXGIInfoManager();
	~CDXGIInfoManager();
	CDXGIInfoManager(const CDXGIInfoManager&) = delete;
	CDXGIInfoManager& operator=(const CDXGIInfoManager&) = delete;
	
	void Set()noexcept;
	std::vector<std::string> GetMessages()const;

private:
	unsigned long long lNext = 0u;
	struct IDXGIInfoQueue* pDXGIInfoQueue = nullptr;
};