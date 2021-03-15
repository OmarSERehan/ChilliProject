#include "Viewport.h"

std::unique_ptr<Viewport> Viewport::CreateObject(float width, float height) noexcept
{
	std::unique_ptr<Viewport> pViewport = std::make_unique<Viewport>();

	std::unique_ptr<D3D11_VIEWPORT> pBuffer = std::make_unique<D3D11_VIEWPORT>();
	{ /// properties relative to render target view
		pBuffer->TopLeftX = 0.0f;
		pBuffer->TopLeftY = 0.0f;

		pBuffer->Width = width;
		pBuffer->Height = height;

		pBuffer->MinDepth = 0.0f;
		pBuffer->MaxDepth = 1.0f;
	}
	pViewport->SetBuffer(std::move(pBuffer));

	return pViewport;
}
bool Viewport::DestroyObject() noexcept
{
	return true;
}

void Viewport::Bind(Graphics* gfx) noexcept
{
	gfx->GetContext()->RSSetViewports(1u, m_pBuffer.get());
}

void Viewport::SetBuffer(std::unique_ptr<D3D11_VIEWPORT> pBuffer) noexcept
{
	m_pBuffer = std::move(pBuffer);
}