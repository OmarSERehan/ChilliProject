#include "VertexBuffer.h"
#include "Graphics.h"

void VertexBuffer::Bind(Graphics* gfx) noexcept
{
	const UINT offset = 0u;
	gfx->GetContext()->IASetVertexBuffers(0u, 1u, m_pBuffer.GetAddressOf(), &m_stride, &offset);
}