#include "DxgiInfoManager.h"

#pragma comment(lib, "dxguid.lib")

DxgiInfoManager::DxgiInfoManager()
{
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	const auto hModDxgiDebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (!hModDxgiDebug)
	{
		std::wstring errorString = L"LoadLibraryEx: Failed";
		MessageBox(nullptr, errorString.c_str(), L"Error loading library", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));
	if (!DxgiGetDebugInterface)
	{
		std::wstring errorString = L"GetProcAddress: Failed";
		MessageBox(nullptr, errorString.c_str(), L"Error initializing dxgi info manager", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &m_pDxgiInfoQueue);
	if (!DxgiGetDebugInterface)
	{
		std::wstring errorString = L"Something: Failed";
		MessageBox(nullptr, errorString.c_str(), L"Error initializing dxgi info manager", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
}

void DxgiInfoManager::Mark() noexcept
{
	// set the index (next) so that the next all to GetMessages() will only get errors generated after this call
	next = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = m_pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		// get the size of message i in bytes
		SIZE_T messageLength;
		HRESULT result = m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);
		if (FAILED(result) == TRUE)
		{
			// TODO: add msgbox
			return std::vector<std::string>();
		}

		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message and push its description into the vector
		result = m_pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength);
		if (FAILED(result) == TRUE)
		{
			// TODO: add msgbox
			return std::vector<std::string>();
		}

		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}