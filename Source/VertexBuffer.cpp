#include "VertexBuffer.h"
#include "Graphics.h"

VertexBuffer::~VertexBuffer()
{
	m_pBuffer->Release();
}

void VertexBuffer::Bind(Graphics* gfx) noexcept
{
	const UINT offset = 0u;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = gfx->GetContext();
	pContext->IASetVertexBuffers(0u, 1u, m_pBuffer.GetAddressOf(), &m_stride, &offset);
}