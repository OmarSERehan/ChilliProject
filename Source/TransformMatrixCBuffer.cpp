#include "TransformMatrixCBuffer.h"

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformMatrixCBuffer::s_pCBuffer = nullptr;

TransformMatrixCBuffer::TransformMatrixCBuffer(std::shared_ptr<Graphics> pGfx, IDrawable* pParent)
	:
	m_pParent(pParent)
{
	/// Only initialized once
	if (!s_pCBuffer)
	{
		s_pCBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(pGfx);
	}
}

void TransformMatrixCBuffer::Bind(std::shared_ptr<Graphics> pGfx) noexcept
{
	s_pCBuffer->Update(pGfx, DirectX::XMMatrixTranspose(m_pParent->GetModelMatrix() * pGfx->GetProjection()));
	s_pCBuffer->Bind(pGfx);
}
