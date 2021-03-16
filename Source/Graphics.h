#pragma once
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

#include "ErrorHandler.h"
#include "DxgiInfoManager.h"
#include "WinHeader.h"

class Graphics
{
public:
	Graphics(HWND windowHandle);

	void SwapFrames();

	DirectX::XMMATRIX GetProjection() noexcept;
	void SetProjection(DirectX::XMMATRIX projMatrix) noexcept;

	void DrawIndexed(uint32_t count);
	
	void ClearBackBuffer(float r, float g, float b) noexcept;
	void DrawTestTriangle(float angle, float x, float y);
	void DrawTestCube(float angle, float x, float y) noexcept;


	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const;
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() const;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() const;


private:
	const float m_inverseAspectRatio = 0.75f;
	DirectX::XMMATRIX m_projMatrix = DirectX::XMMATRIX();

	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthBufferDSV = nullptr;

#ifndef NDEBUG
	DxgiInfoManager dxgiInfoManager;
#endif


	bool m_bFirstPass = true;


public:
	Graphics(const Graphics&) = delete;
	Graphics& operator = (const Graphics&) = delete;

	Graphics(Graphics&&) = delete;
	Graphics& operator = (Graphics&&) = delete;
};