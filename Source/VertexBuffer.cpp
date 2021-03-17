#include "VertexBuffer.h"
#include "Graphics.h"


void VertexBuffer::Bind(std::shared_ptr<Graphics> ppGfx) noexcept
{
	const UINT offset = 0u;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = ppGfx->GetContext();
	pContext->IASetVertexBuffers(0u, 1u, m_pBuffer.GetAddressOf(), &m_stride, &offset);
}