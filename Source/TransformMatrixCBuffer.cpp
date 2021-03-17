#include "TransformMatrixCBuffer.h"


TransformMatrixCBuffer::TransformMatrixCBuffer(std::shared_ptr<Graphics> pGfx, IDrawable* pParent)
	:
	m_pCBuffer(VertexConstantBuffer<DirectX::XMMATRIX>(pGfx)),
	m_pParent(pParent)
{}

void TransformMatrixCBuffer::Bind(std::shared_ptr<Graphics> pGfx) noexcept
{
	m_pCBuffer.Update(pGfx, DirectX::XMMatrixTranspose(m_pParent->GetModelMatrix() * pGfx->GetProjection()));
	m_pCBuffer.Bind(pGfx);
}
