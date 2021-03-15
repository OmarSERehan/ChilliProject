#include "TransformMatrixCBuffer.h"


std::unique_ptr<TransformMatrixCBuffer> TransformMatrixCBuffer::CreateObject(Graphics* gfx, IDrawable* pParent) noexcept
{
	std::unique_ptr<TransformMatrixCBuffer> pTransformMatrixCBuffer = std::make_unique<TransformMatrixCBuffer>();
	pTransformMatrixCBuffer->SetConstantBufferObject(VertexConstantBuffer<DirectX::XMMATRIX>::CreateObject(gfx));
	pTransformMatrixCBuffer->SetParent(pParent);

	return pTransformMatrixCBuffer;
}
bool TransformMatrixCBuffer::DestroyObject() noexcept
{
	return true;
}

void TransformMatrixCBuffer::Bind(Graphics* gfx) noexcept
{
	m_pCBuffer->Update(gfx, DirectX::XMMatrixTranspose(m_pParent->GetModelMatrix() * gfx->GetProjection()));
	m_pCBuffer->Bind(gfx);
}

void TransformMatrixCBuffer::SetConstantBufferObject(std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>>&& pCBuffer)
{
	m_pCBuffer = std::move(pCBuffer);
}
void TransformMatrixCBuffer::SetParent(IDrawable* pParent) noexcept
{
	m_pParent = pParent;
}