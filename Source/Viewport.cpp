#include "Viewport.h"

Viewport::Viewport(float width, float height)
{
	/// properties relative to render target view
	m_pBuffer.TopLeftX = 0.0f;
	m_pBuffer.TopLeftY = 0.0f;

	m_pBuffer.Width = width;
	m_pBuffer.Height = height;

	m_pBuffer.MinDepth = 0.0f;
	m_pBuffer.MaxDepth = 1.0f;
}

void Viewport::Bind(Graphics* gfx) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = gfx->GetContext();
	pContext->RSSetViewports(1u, &m_pBuffer);
}
