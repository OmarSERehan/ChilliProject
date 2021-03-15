#include "TransformMatrixCBuffer.h"


TransformMatrixCBuffer::TransformMatrixCBuffer(Graphics* gfx, IDrawable* pParent)
	:
	m_pCBuffer(VertexConstantBuffer<DirectX::XMMATRIX>(gfx)),
	m_pParent(pParent)
{}

void TransformMatrixCBuffer::Bind(Graphics* gfx) noexcept
{
	m_pCBuffer.Update(gfx, DirectX::XMMatrixTranspose(m_pParent->GetModelMatrix() * gfx->GetProjection()));
	m_pCBuffer.Bind(gfx);
}
