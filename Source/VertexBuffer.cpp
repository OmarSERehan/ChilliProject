#include "VertexBuffer.h"
#include "Graphics.h"


void VertexBuffer::Bind(Graphics* pGfx) noexcept
{
	const UINT offset = 0u;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = pGfx->GetContext();
	pContext->IASetVertexBuffers(0u, 1u, m_pBuffer.GetAddressOf(), &m_stride, &offset);
}