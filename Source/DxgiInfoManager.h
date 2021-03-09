#pragma once
#include <string>
#include <vector>
#include <memory>

#include <Windows.h>
#include <wrl.h>
#include <dxgidebug.h>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;

	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;

	void Mark() noexcept;
	std::vector<std::string> GetMessages() const;

private:
	uint64_t next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> m_pDxgiInfoQueue;
};

