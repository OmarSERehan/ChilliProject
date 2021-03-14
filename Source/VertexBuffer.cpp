#include "VertexBuffer.h"
#include "Graphics.h"


bool VertexBuffer::DestroyObject() noexcept
{
	return true;
}


void VertexBuffer::Bind(Graphics* gfx) noexcept
{
	const UINT offset = 0u;
	gfx->GetContext()->IASetVertexBuffers(0u, 1u, m_pBuffer.GetAddressOf(), &m_stride, &offset);
}


void VertexBuffer::SetBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer) noexcept
{
	m_pBuffer = pBuffer;
}
void VertexBuffer::SetStride(uint32_t stride) noexcept
{
	m_stride = stride;
}