#pragma once
#include <memory>
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>

#include "PopUp.h"
#include "WinHeader.h"
#include "DxgiInfoManager.h"

class Graphics
{
public:
	static std::shared_ptr<Graphics> CreateObject(HWND windowHandle) noexcept;
	bool DestroyObject() noexcept;

	bool SwapFrames() noexcept;
	
	bool ClearBackBuffer(float r, float g, float b) noexcept;
	bool DrawTestTriangle(float angle) noexcept;


	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const;
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() const;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() const;


private:
	float m_inverseAspectRatio = 0.75f;

	void SetDevice(Microsoft::WRL::ComPtr<ID3D11Device> pDevice) noexcept;
	void SetSwapChain(Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain) noexcept;
	void SetContext(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext) noexcept;
	void SetBackBufferRTV(Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pBackBufferRTV) noexcept;

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV = nullptr;

#ifndef NDEBUG
	DxgiInfoManager dxgiInfoManager;
#endif


public:
	Graphics() = default;
	~Graphics() = default;

	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;

	Graphics(Graphics&&) = delete;
	Graphics& operator = (Graphics&&) = delete;
};